#ifndef LOCALTILESOURCE_H
#define LOCALTILESOURCE_H

#include "MapGraphics_global.h"
#include "MapTileSource.h"

class MAPGRAPHICSSHARED_EXPORT LocalTileSource : public MapTileSource
{
    Q_OBJECT
public:
    explicit LocalTileSource(const QString& baseDir, const QString& extension = "jpg");
    virtual ~LocalTileSource();

    // MapTileSource interface
    QPointF ll2qgs(const QPointF& ll, quint8 zoomLevel) const override;
    QPointF qgs2ll(const QPointF& qgs, quint8 zoomLevel) const override;
    quint64 tilesOnZoomLevel(quint8 zoomLevel) const override;
    quint16 tileSize() const override;
    quint8 minZoomLevel(QPointF ll) override;
    quint8 maxZoomLevel(QPointF ll) override;
    QString name() const override;
    QString tileFileExtension() const override;

protected:
    void fetchTile(quint32 x, quint32 y, quint8 z) override;

private:
    void initZoomCache();

    QString _baseDir;
    QString _extension;
    quint8 _minZoom = 0;
    quint8 _maxZoom = 18;
    bool _zoomCached = false;
};

#endif
