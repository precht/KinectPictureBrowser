#pragma once

#include <opencv2/opencv.hpp>
#include <QObject>
#include <QPoint>

class GestureDetector : public QObject
{
    Q_OBJECT
    enum Direction { NONE, LEFT, RIGHT, UP, DOWN };

public:
    explicit GestureDetector(QObject *parent = nullptr);
    void processFrame(QImage& frame);

signals:
    void left();
    void right();
    void up();
    void down();

private:
    void processImage(QImage& frame);
    void findContours(QImage& frame);
    void findBiggestContour();
    void drawCircles(QImage& frame);
    void prepareForNextFrame();
    void investigateMovements();
    bool isValidFrame();
    void calculateDistances();
    void compareDistances();
    void detectedMovement(Direction direction);

    const cv::Point2f m_invalidPosition{-1, -1};
    const cv::Scalar m_green{0, 255, 0, 255};
    const cv::Scalar m_pink{255, 0, 255, 255};
    const cv::Scalar m_orange{0, 165, 255, 255};
    static constexpr int width{640};
    static constexpr int height{480};
    static constexpr int handMinRadius{15};
    static constexpr float precentage{0.5f};

    size_t m_contourIdx{};
    std::vector<std::vector<cv::Point>> m_contours{};
    std::vector<std::vector<cv::Point>> m_contoursPoly{};
    std::vector<cv::Point2f> m_centers{};
    std::vector<float> m_radius{};

    size_t m_framesCounter{};
    cv::Point2f m_movementStartPosition{};
    std::array<cv::Point2f, 10> m_positionHistory{};
    size_t m_historyIdx{};
    const size_t m_historySize{m_positionHistory.size()};

    cv::Point2f m_olderAvgPoint{m_invalidPosition};
    cv::Point2f m_newerAvgPoint{m_invalidPosition};
    float m_horizontalDistance{};
    float m_verticalDistance{};


};
