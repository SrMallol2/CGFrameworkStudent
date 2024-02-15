#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "GL/glew.h"
#include "../extra/picopng.h"
#include "image.h"
#include "utils.h"
#include "camera.h"
#include "mesh.h"


Image::Image() {
	width = 0; height = 0;
	pixels = NULL;
}

Image::Image(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new Color[width*height];
	memset(pixels, 0, width * height * sizeof(Color));
}

// Copy constructor
Image::Image(const Image& c)
{
	pixels = NULL;
	width = c.width;
	height = c.height;
	bytes_per_pixel = c.bytes_per_pixel;
	if(c.pixels)
	{
		pixels = new Color[width*height];
		memcpy(pixels, c.pixels, width*height*bytes_per_pixel);
	}
}

// Assign operator
Image& Image::operator = (const Image& c)
{
	if(pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	bytes_per_pixel = c.bytes_per_pixel;

	if(c.pixels)
	{
		pixels = new Color[width*height*bytes_per_pixel];
		memcpy(pixels, c.pixels, width*height*bytes_per_pixel);
	}
	return *this;
}

Image::~Image()
{
	if(pixels) 
		delete pixels;
}

void Image::Render()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(width, height, bytes_per_pixel == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

// Change image size (the old one will remain in the top-left corner)
void Image::Resize(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for(unsigned int x = 0; x < min_width; ++x)
		for(unsigned int y = 0; y < min_height; ++y)
			new_pixels[ y * width + x ] = GetPixel(x,y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

// Change image size and scale the content
void Image::Scale(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];

	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++y)
			new_pixels[ y * width + x ] = GetPixel((unsigned int)(this->width * (x / (float)width)), (unsigned int)(this->height * (y / (float)height)) );

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

Image Image::GetArea(unsigned int start_x, unsigned int start_y, unsigned int width, unsigned int height)
{
	Image result(width, height);
	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++x)
		{
			if( (x + start_x) < this->width && (y + start_y) < this->height) 
				result.SetPixel( x, y, GetPixel(x + start_x,y + start_y) );
		}
	return result;
}

void Image::FlipY()
{
	int row_size = bytes_per_pixel * width;
	Uint8* temp_row = new Uint8[row_size];
#pragma omp simd
	for (int y = 0; y < height * 0.5; y += 1)
	{
		Uint8* pos = (Uint8*)pixels + y * row_size;
		memcpy(temp_row, pos, row_size);
		Uint8* pos2 = (Uint8*)pixels + (height - y - 1) * row_size;
		memcpy(pos, pos2, row_size);
		memcpy(pos2, temp_row, row_size);
	}
	delete[] temp_row;
}

bool Image::LoadPNG(const char* filename, bool flip_y)
{
	std::string sfullPath = absResPath(filename);
	std::ifstream file(sfullPath, std::ios::in | std::ios::binary | std::ios::ate);

	// Get filesize
	std::streamsize size = 0;
	if (file.seekg(0, std::ios::end).good()) size = file.tellg();
	if (file.seekg(0, std::ios::beg).good()) size -= file.tellg();

	if (!size)
		return false;

	std::vector<unsigned char> buffer;

	// Read contents of the file into the vector
	if (size > 0)
	{
		buffer.resize((size_t)size);
		file.read((char*)(&buffer[0]), size);
	}
	else
		buffer.clear();

	std::vector<unsigned char> out_image;

	if (decodePNG(out_image, width, height, buffer.empty() ? 0 : &buffer[0], (unsigned long)buffer.size(), true) != 0)
		return false;

	size_t bufferSize = out_image.size();
	unsigned int originalBytesPerPixel = (unsigned int)bufferSize / (width * height);
	
	// Force 3 channels
	bytes_per_pixel = 3;

	if (originalBytesPerPixel == 3) {
		pixels = new Color[bufferSize];
		memcpy(pixels, &out_image[0], bufferSize);
	}
	else if (originalBytesPerPixel == 4) {

		unsigned int newBufferSize = width * height * bytes_per_pixel;
		pixels = new Color[newBufferSize];

		unsigned int k = 0;
		for (unsigned int i = 0; i < bufferSize; i += originalBytesPerPixel) {
			pixels[k] = Color(out_image[i], out_image[i + 1], out_image[i + 2]);
			k++;
		}
	}

	// Flip pixels in Y
	if (flip_y)
		FlipY();

	return true;
}

// Loads an image from a TGA file
bool Image::LoadTGA(const char* filename, bool flip_y)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char TGAcompare[12];
	unsigned char header[6];
	unsigned int imageSize;
	unsigned int bytesPerPixel;

    std::string sfullPath = absResPath( filename );

	FILE * file = fopen( sfullPath.c_str(), "rb");
   	if ( file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
		fread(header, 1, sizeof(header), file) != sizeof(header))
	{
		std::cerr << "File not found: " << sfullPath.c_str() << std::endl;
		if (file == NULL)
			return NULL;
		else
		{
			fclose(file);
			return NULL;
		}
	}

	TGAInfo* tgainfo = new TGAInfo;
    
	tgainfo->width = header[1] * 256 + header[0];
	tgainfo->height = header[3] * 256 + header[2];
    
	if (tgainfo->width <= 0 || tgainfo->height <= 0 || (header[4] != 24 && header[4] != 32))
	{
		fclose(file);
		delete tgainfo;
		return NULL;
	}
    
	tgainfo->bpp = header[4];
	bytesPerPixel = tgainfo->bpp / 8;
	imageSize = tgainfo->width * tgainfo->height * bytesPerPixel;
    
	tgainfo->data = new unsigned char[imageSize];
    
	if (tgainfo->data == NULL || fread(tgainfo->data, 1, imageSize, file) != imageSize)
	{
		if (tgainfo->data != NULL)
			delete tgainfo->data;
            
		fclose(file);
		delete tgainfo;
		return false;
	}

	fclose(file);

	// Save info in image
	if(pixels)
		delete pixels;

	width = tgainfo->width;
	height = tgainfo->height;
	pixels = new Color[width*height];

	// Convert to float all pixels
	for (unsigned int y = 0; y < height; ++y) {
		for (unsigned int x = 0; x < width; ++x) {
			unsigned int pos = y * width * bytesPerPixel + x * bytesPerPixel;
			// Make sure we don't access out of memory
			if( (pos < imageSize) && (pos + 1 < imageSize) && (pos + 2 < imageSize))
				SetPixel(x, height - y - 1, Color(tgainfo->data[pos + 2], tgainfo->data[pos + 1], tgainfo->data[pos]));
		}
	}

	// Flip pixels in Y
	if (flip_y)
		FlipY();

	delete tgainfo->data;
	delete tgainfo;

	return true;
}

// Saves the image to a TGA file
bool Image::SaveTGA(const char* filename)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	std::string fullPath = absResPath(filename);
	FILE *file = fopen(fullPath.c_str(), "wb");
	if ( file == NULL )
	{
		perror("Failed to open file: ");
		return false;
	}

	unsigned short header_short[3];
	header_short[0] = width;
	header_short[1] = height;
	unsigned char* header = (unsigned char*)header_short;
	header[4] = 24;
	header[5] = 0;

	fwrite(TGAheader, 1, sizeof(TGAheader), file);
	fwrite(header, 1, 6, file);

	// Convert pixels to unsigned char
	unsigned char* bytes = new unsigned char[width*height*3];
	for(unsigned int y = 0; y < height; ++y)
		for(unsigned int x = 0; x < width; ++x)
		{
			Color c = pixels[y*width+x];
			unsigned int pos = (y*width+x)*3;
			bytes[pos+2] = c.r;
			bytes[pos+1] = c.g;
			bytes[pos] = c.b;
		}

	fwrite(bytes, 1, width*height*3, file);
	fclose(file);

	return true;
}

void Image::DrawImage(const Image& image, int x, int y, bool top) {
	/*
	if (top) {
		 y_0 = this->height - y;
	}*/

	int imWidth = image.width;
	int imHeight = image.height;

	for (int i = 0; i <= imWidth; ++i) {
		for (int j = 0; j <= imHeight; ++j) {
			Color pixelColor = image.GetPixelSafe(i,j); 
			SetPixelSafe(x+i, y+j, pixelColor);

		}
	}
	
  
}



#include <cmath>

void Image::DrawCircle(int x, int y, int r, const Color& borderColor, int borderWidth, bool isFilled, const Color& fillColor) {
	int cx = x;
	int cy = y;
	int bw = borderWidth;
	int radius = r;
	int xCoord, yCoord;
	int d;

	if (isFilled) {
		for (int i = -radius; i <= radius; ++i) {
			for (int j = -radius; j <= radius; ++j) {
				if (i * i + j * j <= radius * radius) {
					SetPixelSafe(cx + i, cy + j, fillColor);
				}
			}
		}
	}

	xCoord = radius;
	yCoord = 0;
	d = 1 - radius;

	while (xCoord >= yCoord) {
		// Drawing the circle
		for (int i = -radius-bw; i <= radius+bw; ++i) {
			for (int j = -radius-bw; j <= radius+bw; ++j) {
				if (i * i + j * j <= (radius + bw) * (radius + bw) &&
					i * i + j * j >= (radius - bw) * (radius - bw)) {
					SetPixelSafe(cx + i, cy + j, borderColor);
				}
			}
		}
		++yCoord;

		if (d <= 0) {
			d = d + 2 * yCoord + 1;
		}
		else {
			xCoord = xCoord - 1;
			d = d + 2 * (yCoord - xCoord) + 1;
		}
	}
}


void Image::DrawRect(int x0, int y0, int w, int h, const Color& borderColor,
	int borderWidth, bool isFilled, const Color& fillColor)
{

	if (isFilled) {
		for (int x = x0 + borderWidth; x < (x0 + w - borderWidth); ++x)
		{
			for (int y = y0+ borderWidth; y < (y0 + h- borderWidth); ++y)
			{
				SetPixelSafe(x, y, fillColor);
			}
		}
	}
	// Draw only the border
	 for (int x = x0; x < (x0 + w); ++x)
	 {
		 for (int y = y0; y < (y0 + borderWidth); ++y)
		 {
			 SetPixelSafe(x, y, borderColor);
			 SetPixelSafe(x, y +h- borderWidth, borderColor);
		 }
	 }
	 for (int x = x0; x < (x0 + borderWidth); ++x)
	 {
		 for (int y = y0; y < (y0 + h); ++y)
		 {
			 SetPixelSafe(x, y, borderColor);
			 SetPixelSafe(x+w-borderWidth, y, borderColor);
		 }
	 }
}

void Image::ScanLineDDA(int x0, int y0, int x1, int y1,
	std::vector<Cell>& table) {
	int dx = x1 - x0;
	int dy = y1 - y0;

	float d = std::max(std::abs(dx), std::abs(dy));
	float directionvect[2] = { dx / d,dy / d };

	float x = static_cast<float>(x0);
	float y = static_cast<float>(y0);

	for (int i = 0; i <= d; ++i) {

		if (table[y].minx > x) {
			table[y].minx = x;
		}
		if (table[y].maxx < x) {
			table[y].maxx = x;
		}
		x += directionvect[0];
		y += directionvect[1];

	}


}

void Image::DrawTriangle(const Vector2& p0, const Vector2& p1, const Vector2& p2,
	const Color& borderColor, bool isFilled, const Color& fillColor) {
	
	if (isFilled) {
		std::vector<Cell> AET;
		AET.resize(this->height + 1);
		// Scan min and max width values using the line rasterization algorithm
		ScanLineDDA(p0.x, p0.y, p1.x, p1.y, AET);
		ScanLineDDA(p1.x, p1.y, p2.x, p2.y, AET);
		ScanLineDDA(p2.x, p2.y, p0.x, p0.y, AET);

		for (int y = 0; y <= this->height; ++y) {
			if (AET[y].minx < AET[y].maxx) {
				for (int x = AET[y].minx; x <= AET[y].maxx; ++x) {
					SetPixelSafe(x, y, fillColor);
				}
			}
		}
	}
	DrawLineDDA(p0.x, p0.y, p1.x, p1.y, borderColor);
	DrawLineDDA(p1.x, p1.y, p2.x, p2.y, borderColor);
	DrawLineDDA(p2.x, p2.y, p0.x, p0.y, borderColor);



}

void Image::DrawTriangleInterpolated(const Vector3& p0, const Vector3& p1, const Vector3& p2,
	const Color& c0, const Color& c1, const Color& c2, FloatImage* zBuffer,
	Image* texture, const Vector2& uv0, const Vector2& uv1, const Vector2& uv2,  int renderMode) {

		std::vector<Cell> AET;
		AET.resize(this->height + 1);
		// Scan min and max width values using the line rasterization algorithm
		ScanLineDDA(p0.x, p0.y, p1.x, p1.y, AET);
		ScanLineDDA(p1.x, p1.y, p2.x, p2.y, AET);
		ScanLineDDA(p2.x, p2.y, p0.x, p0.y, AET);

		Matrix44 m;
		m.M[0][0] = p0.x;
		m.M[0][1] = p1.x;
		m.M[0][2] = p2.x;
		
		m.M[1][0] = p0.y;
		m.M[1][1] = p1.y;
		m.M[1][2] = p2.y;
		
		m.M[2][0] = 1;
		m.M[2][1] = 1;
		m.M[2][2] = 1;
		
		m.Transpose();
	    m.Inverse();

		Vector3 bCoords;
	    Color finalColor;
		
		
			for (int y = 0; y <= this->height; ++y) {
				if (AET[y].minx < AET[y].maxx) {
					for (int x = AET[y].minx; x <= AET[y].maxx; ++x) {
						bCoords = m * Vector3(x, y, 1);

						bCoords.Clamp(0, 1);
						float sum = bCoords.x + bCoords.y + bCoords.z;
						bCoords = bCoords / sum;


						float interpolatedZ = bCoords.x * p0.z + bCoords.y * p1.z + bCoords.z * p2.z;


						if (interpolatedZ < zBuffer->GetPixel(x, y)) {
							// Update Z-buffer
							if (renderMode != 3) {
								zBuffer->SetPixel(x, y, interpolatedZ);
							}
							
							if (texture == nullptr || (renderMode==1 || renderMode==3)) {
								

								// Interpolate color
								finalColor = bCoords.x * c0 + bCoords.y * c1 + bCoords.z * c2;

								SetPixelSafe(x, y, finalColor);
							}

							else if(texture != nullptr && renderMode == 2) {
							//use texture

							Vector2 uv0_ts = Vector2((uv0.x * texture->width - 1), (uv0.y * texture->height - 1));
							Vector2 uv1_ts = Vector2((uv1.x * texture->width - 1), (uv1.y * texture->height - 1));
							Vector2 uv2_ts = Vector2((uv2.x * texture->width - 1), (uv2.y * texture->height - 1));

							float uv_x = (uv0_ts.x * bCoords.x) + (uv1_ts.x * bCoords.y) + (uv2_ts.x * bCoords.z);
							float uv_y = (uv0_ts.y * bCoords.x) + (uv1_ts.y * bCoords.y) + (uv2_ts.y * bCoords.z);

							Color c_texture = texture->GetPixelSafe(uv_x, uv_y);

							SetPixelSafe(x, y, c_texture);
						}

						}
						

						

					}
				}
			}
		}
		
	



#ifndef IGNORE_LAMBDAS

// You can apply and algorithm for two images and store the result in the first one
// ForEachPixel( img, img2, [](Color a, Color b) { return a + b; } );
template <typename F>
void ForEachPixel(Image& img, const Image& img2, F f) {
	for(unsigned int pos = 0; pos < img.width * img.height; ++pos)
		img.pixels[pos] = f( img.pixels[pos], img2.pixels[pos] );
}

#endif

FloatImage::FloatImage(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new float[width * height];
	memset(pixels, 0, width * height * sizeof(float));
}

// Copy constructor
FloatImage::FloatImage(const FloatImage& c) {
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new float[width * height];
		memcpy(pixels, c.pixels, width * height * sizeof(float));
	}
}

// Assign operator
FloatImage& FloatImage::operator = (const FloatImage& c)
{
	if (pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new float[width * height * sizeof(float)];
		memcpy(pixels, c.pixels, width * height * sizeof(float));
	}
	return *this;
}

FloatImage::~FloatImage()
{
	if (pixels)
		delete pixels;
}

// Change image size (the old one will remain in the top-left corner)
void FloatImage::Resize(unsigned int width, unsigned int height)
{
	float* new_pixels = new float[width * height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for (unsigned int x = 0; x < min_width; ++x)
		for (unsigned int y = 0; y < min_height; ++y)
			new_pixels[y * width + x] = GetPixel(x, y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}





void Image::DrawLineDDA(int x0, int y0, int x1, int y1, const Color& c) {
	
	int dx = x1 - x0;
	int dy = y1 - y0;

	float d = std::max(std::abs(dx), std::abs(dy));
	float directionvect[2] = {dx/d,dy/d};

	float x = static_cast<float>(x0);
	float y = static_cast<float>(y0);
	
	for (int i = 0; i <= d; ++i) {
		SetPixelSafe(static_cast<int>(std::floor(x)), static_cast<int>(std::floor(y)), c);
		x += directionvect[0];
		y += directionvect[1];

	}

}
//BUTTON CLASS METHODS

Button::Button(unsigned int x0, unsigned int y0) {
	this->x0 = x0;
	this->y0 = y0;
	this->image = NULL;
}
void Button::SetImage(const char* filename) {
	Image* buttonImage = new Image;
	buttonImage->LoadPNG(filename);
	this->image = buttonImage;
}

bool Button::IsMouseInside(Vector2 mousePosition) {
	int x0 = this->x0;
	int y0 = this->y0;
	int x1 = this->image->width- x0;
	int y1 = this->image->height - y0;
	
	if ((mousePosition.x >= x0 && mousePosition.x <= x1) &&
		(mousePosition.y >= y0 && mousePosition.y <= y1)) {
		return true;
	}
	else return false;
	
}
void Button::DrawButton(const Button& button, int x, int y) {
	
}
