#include "tileSetImage.h"

TileSetImage::TileSetImage(const char* source, const int& width, const int height): source_(source),
			 width_(width),
			 height_(height)
{
}

TileSetImage::~TileSetImage()
{
}

const char* TileSetImage::getSource() const
{
	return source_;
}

int TileSetImage::getWidth() const
{
	return width_;
}

int TileSetImage::getHeight() const
{
	return height_;
}
