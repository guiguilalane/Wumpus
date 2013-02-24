#ifndef TILESETIMAGE_H
#define TILESETIMAGE_H

/*!
*	\file tileSetImage.h
*	\brief the TileSet image representation
*	\author Guillaume COUTABLE
*	\version 1.0
*/

/**
 * @class TileSetImage
 * @brief the TileSetImage class
 *
 */
class TileSetImage{

	private:
        const char* source_; /**< the image path that represent the tileset */
        const int width_; /**< the image width */
        const int height_; /**< the image height */
		
	public:

        /*!
         * \brief constructor
         *
         *  TileSetImage constructor
         *
         * \param source the image path that represent the tileset
         * \param width the image width
         * \param height the image height
         */
		TileSetImage(const char* source, const int& width, const int height);

        /**
         * @brief Destructor
         *
         *  TileSetImage destructor
         *
         */
        ~TileSetImage();
		
        /**
         * @brief the image source accessor
         *
         * @return the image path
         */
        const char* getSource() const;

        /**
         * @brief the image width accessor
         *
         * @return the image width
         */
        int getWidth() const;

        /**
         * @brief the image height accessor
         *
         * @return the image height
         */
        int getHeight() const;

};

#endif
