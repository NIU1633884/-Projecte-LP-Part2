// BallTree.cpp
#include "pch.h"
// El resto de tus includes y código sigue aquí.

#include "BallTree.h"
#include <limits>


Coordinate findMedian(const std::vector<Coordinate>& coordenades) {
    // Make a copy of the input vector to avoid modifying it
    std::vector<Coordinate> sortedCoords = coordenades;

    // Sort the vector based on the custom comparison operators
    std::sort(sortedCoords.begin(), sortedCoords.end());

    // Find the median element
    size_t medianIndex = sortedCoords.size() / 2;

    return sortedCoords[medianIndex];
}


void BallTree::construirArbre(const std::vector<Coordinate>& coordenades) {
    // Handle the base case where coordenades is empty
    if (coordenades.empty()) {
        return;
    }

    // Choose a pivot (for simplicity, choose the median point)
    // Note: You may need a function to find a suitable pivot based on your requirements
    Coordinate pivot = findMedian(coordenades);

    // Create vectors for left and right subtrees
    std::vector<Coordinate> leftCoords, rightCoords;

    // Partition coordenades around the pivot
    for (const auto& coord : coordenades) {
        if (coord < pivot) { // You'll need to define this comparison
            leftCoords.push_back(coord);
        }
        else if (coord > pivot) {
            rightCoords.push_back(coord);
        }
    }

    // Recursively build left and right subtrees
    this->m_left = new BallTree();
    this->m_right = new BallTree();
    this->m_left->construirArbre(leftCoords);
    this->m_right->construirArbre(rightCoords);
}

void BallTree::inOrdre(std::vector<std::list<Coordinate>>& out) {
    if (this->m_left != nullptr) {
        this->m_left->inOrdre(out);
    }

    std::list<Coordinate> currentCoords;
    currentCoords.insert(currentCoords.end(), this->m_coordenades.begin(), this->m_coordenades.end());
    out.push_back(currentCoords);

    if (this->m_right != nullptr) {
        this->m_right->inOrdre(out);
    }
}

void BallTree::preOrdre(std::vector<std::list<Coordinate>>& out) {
    std::list<Coordinate> currentCoords;
    currentCoords.insert(currentCoords.end(), this->m_coordenades.begin(), this->m_coordenades.end());
    out.push_back(currentCoords);

    if (this->m_left != nullptr) {
        this->m_left->preOrdre(out);
    }
    if (this->m_right != nullptr) {
        this->m_right->preOrdre(out);
    }
}

void BallTree::postOrdre(std::vector<std::list<Coordinate>>& out) {
    if (this->m_left != nullptr) {
        this->m_left->postOrdre(out);
    }
    if (this->m_right != nullptr) {
        this->m_right->postOrdre(out);
    }

    std::list<Coordinate> currentCoords;
    currentCoords.insert(currentCoords.end(), this->m_coordenades.begin(), this->m_coordenades.end());
    out.push_back(currentCoords);
}

double distance(const Coordinate& a, const Coordinate& b) {
    // Simple Euclidean distance for demonstration purposes
    double latDiff = a.lat - b.lat;
    double lonDiff = a.lon - b.lon;
    return sqrt(latDiff * latDiff + lonDiff * lonDiff);
}
Coordinate BallTree::nodeMesProper(Coordinate targetQuery, Coordinate& Q, BallTree* ball) {
    // Check if the current ball (node) is null
    if (ball == nullptr) {
        // Return an invalid coordinate to indicate that no further search is possible
        return Coordinate{ -1, -1 };
    }

    // Calculate the distance from the targetQuery to the current node's pivot
    double currentDistance = distance(targetQuery, ball->getPivot());

    // If this distance is smaller than the distance to the best candidate found so far (Q),
    // then update Q with the current node's pivot
    if (currentDistance < distance(targetQuery, Q)) {
        Q = ball->getPivot();
    }

    // Recursively search the left and right subtrees
    Coordinate leftClosest = Coordinate{ -1, -1 }, rightClosest = Coordinate{ -1, -1 };
    if (ball->getEsquerre() != nullptr) {
        leftClosest = nodeMesProper(targetQuery, Q, ball->getEsquerre());
    }
    if (ball->getDreta() != nullptr) {
        rightClosest = nodeMesProper(targetQuery, Q, ball->getDreta());
    }

    // Determine which of leftClosest, rightClosest, or Q is the nearest to targetQuery
    auto leftDistance = (leftClosest.lat != -1) ? distance(targetQuery, leftClosest) : (std::numeric_limits<double>::max)();
    auto rightDistance = (rightClosest.lat != -1) ? distance(targetQuery, rightClosest) : (std::numeric_limits<double>::max)();

    if (leftDistance < currentDistance && leftDistance <= rightDistance) {
        return leftClosest;
    }
    else if (rightDistance < currentDistance) {
        return rightClosest;
    }
    else {
        return Q;
    }
}
