#include "LocalTileSource.h"

#include <QDir>
#include <QImage>
#include <QStringBuilder>
#include <QtDebug>
#include <cmath>

static const qreal PI = 3.14159265358979323846;
static const qreal DEG2RAD = PI / 180.0;
static const qreal RAD2DEG = 180.0 / PI;

LocalTileSource::LocalTileSource(const QString& baseDir, const QStringList& extensions) :
    MapTileSource(), _baseDir(baseDir), _extensions(extensions)
{
    this->setCacheMode(MapTileSource::NoCaching);
    initZoomCache();
}

LocalTileSource::~LocalTileSource()
{
    qDebug() << this << "LocalTileSource destructing";
}

QPointF LocalTileSource::ll2qgs(const QPointF& ll, quint8 zoomLevel) const
{
    const qreal tilesOnOneEdge = static_cast<qreal>(1ULL << zoomLevel);
    const quint16 tileSize = this->tileSize();
    qreal x = (ll.x() + 180.0) * (tilesOnOneEdge * tileSize) / 360.0;
    qreal y = (1.0 - (log(tan(PI / 4.0 + (ll.y() * DEG2RAD) / 2.0)) / PI)) / 2.0 *
              (tilesOnOneEdge * tileSize);
    return QPoint(int(x), int(y));
}

QPointF LocalTileSource::qgs2ll(const QPointF& qgs, quint8 zoomLevel) const
{
    const qreal tilesOnOneEdge = static_cast<qreal>(1ULL << zoomLevel);
    const quint16 tileSize = this->tileSize();
    qreal longitude = (qgs.x() * (360.0 / (tilesOnOneEdge * tileSize))) - 180.0;
    qreal latitude =
      RAD2DEG * (atan(sinh((1.0 - qgs.y() * (2.0 / (tilesOnOneEdge * tileSize))) * PI)));
    return QPointF(longitude, latitude);
}

quint64 LocalTileSource::tilesOnZoomLevel(quint8 zoomLevel) const
{
    return 1ULL << (2 * zoomLevel);
}

quint16 LocalTileSource::tileSize() const
{
    return 256;
}

void LocalTileSource::initZoomCache()
{
    QDir dir(_baseDir);
    if (!dir.exists())
    {
        _minZoom = 12;
        _maxZoom = 18;
        _zoomCached = true;
        return;
    }

    QStringList entries = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    _minZoom = 18;
    _maxZoom = 0;

    for (const QString& e : entries)
    {
        bool ok = false;
        quint8 z = e.toUInt(&ok);
        if (!ok)
            continue;
        if (z < _minZoom)
            _minZoom = z;
        if (z > _maxZoom)
            _maxZoom = z;
    }

    if (_minZoom > 18)
        _minZoom = 12;
    if (_maxZoom < 12)
        _maxZoom = 18;
    _zoomCached = true;
}

quint8 LocalTileSource::minZoomLevel(QPointF ll)
{
    Q_UNUSED(ll)
    return _minZoom;
}

quint8 LocalTileSource::maxZoomLevel(QPointF ll)
{
    Q_UNUSED(ll)
    return _maxZoom;
}

QString LocalTileSource::name() const
{
    return "Local Tiles";
}

QString LocalTileSource::tileFileExtension() const
{
    return _extensions.first();
}

void LocalTileSource::fetchTile(quint32 x, quint32 y, quint8 z)
{
    for (const auto& ext : _extensions)
    {
        const QString filePath =
          _baseDir % QStringLiteral("/") % QString::number(z) % QStringLiteral("/") %
          QString::number(x) % QStringLiteral("/") % QString::number(y) % QStringLiteral(".") %
          ext;

        QImage* image = new QImage(filePath);
        if (!image->isNull())
        {
            this->prepareNewlyReceivedTile(x, y, z, image, QDateTime::currentDateTime().addYears(1));
            return;
        }
        delete image;
    }

    qDebug() << "LocalTileSource: tile not found" << _baseDir << z << x << y;
}
