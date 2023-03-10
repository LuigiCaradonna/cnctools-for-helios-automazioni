#pragma once

#include <QFile>
#include <QList>
#include <QVector3D>
#include <QVector2D>
#include <QTextStream>
#include <QProgressDialog>
#include <QTranslator>
#include <iostream>
#include "Helpers.h"

class CoordManager : QObject
{
    Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the CoordManager object.
     *
     */
    CoordManager(QObject* parent = Q_NULLPTR);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the CoordManager object.
     */
    ~CoordManager();

    /********** GETTERS **********/

    /*
     * Gets the minimum x value.
     *
     * @return float    - The minimum x value.
     */
    float getXMin();

    /*
     * Gets the minimum y value.
     *
     * @return float    - The minimum y value.
     */
    float getYMin();

    /*
     * Gets the maximum x value.
     *
     * @return float    - The maximum x value.
     */
    float getXMax();

    /*
     * Gets the maximum y value.
     *
     * @return float    - The maximum y value.
     */
    float getYMax();

    /*
     * Gets the maximum z value, z grows negative.
     *
     * @return float    - The maximum z value.
     */
    float getZMax();

    /*
     * Gets the value encoding the up position.
     *
     * @return float    - The value encoding the up position.
     */
    float getUp();

    /*
     * Gets the value encoding the down position.
     *
     * @return float    - The value encoding the down position.
     */
    float getDown();

    /*
     * Gets the value of the offset along the x axis.
     *
     * @return float    - The value of the offset along the x axis.
     */
    float getOffsetX();

    /*
     * Gets the value of the offset along the y axis.
     *
     * @return float    - The value of the offset along the y axis.
     */
    float getOffsetY();

    /*
     * Gets the list of the points where the engraving reaches the maximum depth.
     *
     * @return QList<QVector2D>     - The list of the points where the engraving reaches the maximum depth.
     */
    QList<QVector2D> getZMaxList();

    /*
     * Searches the closest point among those contained into the coordinates loaded from the PGR file(s)
     *
     * @param   const   QList<QVector3D>&   coords
     * @param   const   float               x
     * @param   const   float               y
     *
     * @return QVector3D
     */
    QVector3D getClosestPoint(const QList<QVector3D>& coords, const float x, const float y);


    /********** PUBLIC FUNCTIONS **********/

    /*
     * Translates the provided coordinates along x and y axes by the given amount dx and dy.
     *
     * WARNING: This directly modifies the original vector!
     *
     * @param   QList<QVector3D>&       coords  - List of 3D coordinates to be traslated.
     * @param   int                     dx      - Offset along x axis.
     * @param   int                     dy      - Offset along y axis.
     *
     * @return void
     */
    void translateCoords(QList<QVector3D>& coords, int dx, int dy);

    /*
     * Translates the provided coordinates along x and y axes by the given amount dx and dy.
     *
     * WARNING: This directly modifies the original vector!
     *
     * @param   QList<QVector2D>&       coords  - List of 2D coordinates to be traslated.
     * @param   int                     dx      - Offset along x axis.
     * @param   int                     dy      - Offset along y axis.
     *
     * @return void
     */
    void translateCoords(QList<QVector2D>& coords, int dx, int dy);

    /*
     * Resets the coordinates' min and max values and the offsets.
     *
     * @return void
     */
    void resetCoordinatesLimits();

    /*
     * Reads the ISO files and gets the coordinates useful for the drawing.
     *
     * @param const QStringList&    iso_files   - List containing the paths of the files to be processed
     * @param bool                  sculpture   - Says whether to process a sculpture file or a regular one
     * @param bool                  fit         - Says whether the drawing must fit the canvas or not
     * @param bool                  size_given  - Says whether the slab size was given or not
     *
     * @return QList<QVector3D>     - List of coordinates which will be used for the drawing.
     */
    QList<QVector3D> processCoordinates(const QStringList& iso_files, bool sculpture, bool fit, bool size_given);

private:

    /*
        The vector containing the coordinates consists of float values.
        The following two costants are used to indicate the UP and DOWN positions of the tool.
        Those two values will never appear into the ISO file generated by the software of the machine.
    */

    // Code for the UP position of the tool
    const float UP = 5000.0f;
    // Code for the DOWN position of the tool
    const float DOWN = 6000.0f;

    // Min x position where the drawing will be performed
    float x_min = 10000;
    // Min y position where the drawing will be performed
    float y_min = 10000;
    // Max x position where the drawing will be performed
    float x_max = -10000;
    // Max y position where the drawing will be performed
    float y_max = -10000;
    // Max z position where the drawing will be performed (grows negative)
    float z_max = 0;

    // List of maximum depth points
    QList<QVector2D> z_max_list;

    /*
        Jobs for the machine could also be placed in negative area, but the canvas only accepts
        positive coordinates, thus the drawing must be translated to be visible, the following
        two variables will contain the distance to move the drawing along the x and y axes.
    */

    // Offset along the x axis to get the drawing in the positive area
    int offset_x = 0;
    // Offset along the y axis to get the drawing in the positive area
    int offset_y = 0;
};