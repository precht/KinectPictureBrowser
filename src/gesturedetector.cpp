#include "../include/gesturedetector.h"


#include <opencv2/imgproc.hpp>
#include <QImage>
#include <QDebug>
#include <QDateTime>

GestureDetector::GestureDetector(QObject *parent)
    : QObject(parent)
{ }

void GestureDetector::processFrame(QImage& frame)
{
    prepareForNextFrame();
    findContours(frame);
    investigateMovements();
    drawCircles(frame);
}

//////////////////////////////////////////////////////////////////////////////// FIND HAND POSITION
// https://docs.opencv.org/4.1.2/da/d0c/tutorial_bounding_rects_circles.html

void GestureDetector::prepareForNextFrame()
{
    m_historyIdx = (m_historyIdx + 1) % m_historySize;
    m_positionHistory[m_historyIdx] = m_invalidPosition;
    m_contours.clear();
}

void GestureDetector::findContours(QImage& frame)
{
    auto gray = frame.convertToFormat(QImage::Format_Grayscale8);
    cv::Mat img = cv::Mat{height, width, CV_8UC1, gray.bits()};
    cv::blur(img, img, cv::Size{2, 2});

    constexpr int thresh = 100;
    cv::Canny(img, img, thresh, thresh>>1);
    cv::findContours(img, m_contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    if (not m_contours.empty())
        findBiggestContour();
}

void GestureDetector::findBiggestContour()
{   
    m_contoursPoly.resize(m_contours.size());
    m_centers.resize(m_contours.size());
    m_radius.resize(m_contours.size());

    m_contourIdx = 0;
    float maxRadius = 0;
    for (size_t i = 0; i < m_contours.size(); i++)
    {
        cv::approxPolyDP(m_contours[i], m_contoursPoly[i], 3, true);
        cv::minEnclosingCircle(m_contoursPoly[i], m_centers[i], m_radius[i]);
        if (m_radius[i] > maxRadius)
        {
            m_contourIdx = i;
            maxRadius = m_radius[i];
        }
    }
    if (m_radius[m_contourIdx] < handMinRadius)
        return;
    m_positionHistory[m_historyIdx] = m_centers[m_contourIdx];
}

void GestureDetector::drawCircles(QImage& frame)
{
    cv::Mat rst{height, width, CV_8UC4, frame.bits()};

    // draw lines between old positions
    auto startIdx = (m_historyIdx + 1) % m_historySize;
    auto endIdx = (m_historyIdx + 2) % m_historySize;
    for (size_t i{0}; i < m_historySize; i++)
    {
        const auto& startPoint = m_positionHistory[startIdx];
        const auto& endPoint = m_positionHistory[endIdx];

        endIdx = startIdx;
        startIdx = (startIdx + 1) % m_historySize;

        if (startPoint == m_invalidPosition
                or endPoint == m_invalidPosition)
            continue;

        cv::line(rst, startPoint, endPoint, m_orange, 2);
    }

    // draw cirles around the hand
    if (not m_contours.empty())
    {
        cv::circle(rst, m_centers[m_contourIdx], 4, m_orange, 8);
        cv::circle(rst, m_centers[m_contourIdx], static_cast<int>(m_radius[m_contourIdx]), m_pink, 3);
    }
    if (m_framesCounter >= m_historySize
            and m_positionHistory[m_historyIdx] != m_invalidPosition)
    {
//        cv::circle(rst, m_olderAvgPoint, 5, m_orange, 10);
//        cv::circle(rst, m_newerAvgPoint, 5, m_orange, 10);
        cv::arrowedLine(rst, m_olderAvgPoint, m_newerAvgPoint, m_green, 10, 8, 0, 0.3);
    }
    // draw arrow presenting current movement
}

//////////////////////////////////////////////////////////////////////////////// TRACK MOVEMENT

void GestureDetector::investigateMovements()
{
    if (not isValidFrame())
    {
        m_framesCounter = 0;
        return;
    }

    m_framesCounter++;
    if (m_framesCounter < (m_historySize >> 1)) // wait for enough points to perform calculations
        return;

    calculateDistances();

    if (m_framesCounter == (m_historySize >> 1)) // for now we only have one valid point - the start
        m_movementStartPosition = m_newerAvgPoint;
    else
        compareDistances();
}

bool GestureDetector::isValidFrame()
{
    return m_positionHistory[m_historyIdx] != m_invalidPosition;
}

void GestureDetector::calculateDistances()
{
    // divide stored points into two gropus and calculate average position for both
    // then check in which direction hand moved
    m_olderAvgPoint = {0, 0};
    m_newerAvgPoint = {0, 0};
    {
        size_t shift{};
        for (; shift < (m_historySize >> 1); shift++)
            m_olderAvgPoint += m_positionHistory[(m_historyIdx + shift) % m_historySize];
        for (; shift < m_historySize; shift++)
            m_newerAvgPoint += m_positionHistory[(m_historyIdx + shift) % m_historySize];
        const auto olderCount = m_historySize >> 1;
        m_olderAvgPoint.x /= olderCount;
        m_olderAvgPoint.y /= olderCount;
        const auto newerCount = m_historySize - olderCount;
        m_newerAvgPoint.x /= newerCount;
        m_newerAvgPoint.y /= newerCount;
    }
    m_horizontalDistance = m_movementStartPosition.x - m_positionHistory[m_historyIdx].x;
    m_verticalDistance = m_movementStartPosition.y - m_positionHistory[m_historyIdx].y;
}

void GestureDetector::compareDistances()
{
    if (m_framesCounter < m_historySize) // still too few points
        return;

    if (std::abs(m_horizontalDistance) > width * precentage)
    {
        if (m_horizontalDistance < 0)
            detectedMovement(RIGHT);
        else
            detectedMovement(LEFT);
    }
    else if (std::abs(m_verticalDistance) > height * precentage)
    {
        if (m_verticalDistance < 0)
            detectedMovement(DOWN);
        else
            detectedMovement(UP);
    }
}

void GestureDetector::detectedMovement(GestureDetector::Direction direction)
{
    switch (direction)
    {
    case RIGHT:
        qInfo() << "RIGHT";
        emit right();
        break;
    case LEFT:
        qInfo() << "LEFT";
        emit left();
        break;
    case UP:
        qInfo() << "UP";
        emit up();
        break;
    case DOWN:
        qInfo() << "DOWN";
        emit down();
        break;
    case NONE:
        break;
    }
    m_framesCounter = 0;
}
