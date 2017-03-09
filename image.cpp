#include "image.h"

CImage::CImage(QImage* img)
{
	width = img->width();
	height = img->height();

	pos = nullptr;
	pos = new int*[width];
	for(int x = 0; x < width; ++x) {
		pos[x] = new int[height];
	}

	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			pos[x][y] = qGray(img->pixel(x, y));
		}
	}
}

CImage::CImage(CImage* img)
{
   width = img->width;
	height = img->height;

	pos = 0;
	pos = new int*[width];
	for(int x = 0; x < width; ++x) {
		pos[x] = new int[height];
	}

	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			pos[x][y] = img->pos[x][y];
		}
	}
}

void CImage::integrate() 
{
	for(int y = 1; y < height; ++y)
		pos[0][y] = pos[0][y-1] + pos[0][y];
	
	for(int x = 1; x < width; ++x)
		pos[x][0] = pos[x-1][0] + pos[x][0];

	for(int y = 1; y < height; ++y) {
		for(int x = 1; x < width; ++x) {
			pos[x][y] = pos[x][y-1] + pos[x-1][y] -
							pos[x-1][y-1] + pos[x][y];
		}
	}
}

CImage::~CImage()
{
	if(pos) {
		for(int x = 0; x < width; ++x) {
			delete[] pos[x];
		}

		delete[] pos;
		pos = nullptr;
	}
}

QImage CImage::get()
{
	QImage result(width, height, QImage::Format_RGB32);

	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			int c = pos[x][y];
			if(c > 255) c = 255;
			else if(c < 0) c = 0;
			result.setPixel(x, y, qRgb(c,c,c));
		}
	}

	return result;
}

void CImage::derivateX()
{
	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			int A = x; if(A < 1) A = 0;
			int B = x+1; if(B >= width) B = width-1;

			pos[x][y] = pos[A][y]-pos[B][y];
		}
	}
}

void CImage::derivateY()
{
	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			int A = y; if(A < 1) A = 0;
			int B = y+1; if(B >= height) B = height-1;

			pos[x][y] = pos[x][A]-pos[x][B];
		}
	}
}

void CImage::sumation(int w)
{
	int A = 0;
	int B = 0;
	int C = 0;
	int D = 0;

	CImage igral = new CImage(this);
	igral.integrate();

	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			A = (x+w); if(A >= width) A = width-1;
			B = (y+w); if(B >= height) B = height-1;
			C = (x-w-1);
			D = (y-w-1);

			if((C < 0) && (D < 0))
				pos[x][y] = igral.pos[A][B];
			else if(D < 0)
				pos[x][y] = igral.pos[A][B] - igral.pos[C][B];
			else if(C < 0)
				pos[x][y] = igral.pos[A][B] - igral.pos[A][D];
			else
				pos[x][y] = (igral.pos[A][B]+igral.pos[C][D] -
								 igral.pos[C][B]-igral.pos[A][D]);
		}
	}
}

void CImage::mean(int w)
{
	int W = 0;
	int A = 0;
	int B = 0;
	int C = 0;
	int D = 0;
	int wx = 0;
	int wy = 0;

	CImage igral = new CImage(this);
	igral.integrate();

	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			A = (x+w); if(A >= width) A = width-1;
			B = (y+w); if(B >= height) B = height-1;
			C = (x-w-1); 
			D = (y-w-1);
			wx = A;
			wy = B;

			if((C < 0) && (D < 0)) {
				wx++;
				wy++;
				W = wx*wy;
				pos[x][y] = (igral.pos[A][B])/W;
			}
			else if(D < 0) {
				wy++;
				wx -= C;
				W = wx*wy;
				pos[x][y] = (igral.pos[A][B]-igral.pos[C][B])/W;
			}
			else if(C < 0) {
				wx++;
				wy -= D;
				W = wx*wy;
				pos[x][y] = (igral.pos[A][B]-igral.pos[A][D])/W;
			}
			else {
				wy -= D;
				wx -= C;
				W = wy*wx;
				pos[x][y] = (igral.pos[A][B]+igral.pos[C][D]-
								 igral.pos[C][B]-igral.pos[A][D])/W;
			}
		}
	}
}

void CImage::laplacian()
{
	int W = 0;
	int A = 0;
	int B = 0;
	int C = 0;
	int D = 0;
	int wx = 0;
	int wy = 0;

	CImage igral = new CImage(this);
	igral.integrate();

	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			A = (x+1); if(A >= width) A = width-1;
			B = (y+1); if(B >= height) B = height-1;
			C = (x-2); 
			D = (y-2);
			wx = A;
			wy = B;

			if((C < 0) && (D < 0)) {
				wx++;
				wy++;
				W = wx*wy;
				pos[x][y] = (igral.pos[A][B]) - 
						 		(W * pos[x][y]);
				//pos[x][y] /= W;
			}
			else if(D < 0) {
				wy++;
				wx -= C;
				W = wx*wy;
				pos[x][y] = (igral.pos[A][B]-igral.pos[C][B]) -
						 		(W * pos[x][y]);
				//pos[x][y] /= W;
			}
			else if(C < 0) {
				wx++;
				wy -= D;
				W = wx*wy;
				pos[x][y] = (igral.pos[A][B]-igral.pos[A][D]) -
						 		(W * pos[x][y]);
				//pos[x][y] /= W;
			}
			else {
				wy -= D;
				wx -= C;
				W = wy*wx;
				pos[x][y] = (igral.pos[A][B]+igral.pos[C][D]-
						  		igral.pos[C][B]-igral.pos[A][D]) -
						  		(W * pos[x][y]);
				//pos[x][y] /= W;
			}
		}
	}
}

void CImage::laplacian(int w)
{
	int W = 0;
	int A = 0;
	int B = 0;
	int C = 0;
	int D = 0;
	int wx = 0;
	int wy = 0;

	CImage igral = new CImage(this);
	igral.integrate();

	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			A = (x+w); if(A >= width) A = width-1;
			B = (y+w); if(B >= height) B = height-1;
			C = (x-w-1); 
			D = (y-w-1);
			wx = A;
			wy = B;

			if((C < 0) && (D < 0)) {
				wx++;
				wy++;
				W = wx*wy;
				pos[x][y] = (-1 * (igral.pos[A][B])) + 
						 			(W * pos[x][y]);
				//pos[x][y] /= 24;
			}
			else if(D < 0) {
				wy++;
				wx -= C;
				W = wx*wy;
				pos[x][y] = (-1 * (igral.pos[A][B]-igral.pos[C][B])) +
						 			(W * pos[x][y]);
				//pos[x][y] /= 24;
			}
			else if(C < 0) {
				wx++;
				wy -= D;
				W = wx*wy;
				pos[x][y] = (-1 * (igral.pos[A][B]-igral.pos[A][D])) +
						 			(W * pos[x][y]);
				//pos[x][y] /= 24;
			}
			else {
				wy -= D;
				wx -= C;
				W = wy*wx;
				pos[x][y] = (-1 * (igral.pos[A][B]+igral.pos[C][D]-
						  		igral.pos[C][B]-igral.pos[A][D])) +
						  		(W * pos[x][y]);
				//pos[x][y] /= 24;
			}
		}
	}
}


void CImage::laplacian2()
{
	int Kernel[9];
	int X = 0;
	int Y = 0;
	CImage* src = new CImage(this);

	memset(Kernel, 1, sizeof(Kernel));
	Kernel[0] = 0;
	Kernel[2] = 0;
	Kernel[4] = -4;
	Kernel[6] = 0;
	Kernel[8] = 0;

	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			int sum = 0;	
			int i	  = 0;

			for(int v = -1; v <= 1; ++v) {
				for(int u = -1; u <= 1; ++u) {
					X = x + u;
					Y = y + v;

					sum += src->getValue(X, Y) * Kernel[i];
					i++;
				}
			}

			pos[x][y] = sum;
		}
	}

	delete src;
}

int CImage::getValue(int x, int y) 
{
	if((x >= 0) && (y >= 0) && (x < width) && (y < height))
		return pos[x][y];
	return 0;
}

void CImage::median(int w)
{
	int H[256];
	int center = ((2*w+1)*2+1) / 2;
	int X = 0;
	int Y = 0;
	int A = 0;
	int B = 0;
	int C = 0;
	int D = 0;
	int direction = 1;
	int prev = 0;
	int next = 0;
	int m = 0;
	int n = 0;
	int x = 0;
	int y = 0;
	int median = 0;
	int delta = 0;
	CImage* src = new CImage(this);

	memset(H, 0, sizeof(H));
/*
	for(int i = 0; i < 256; ++i)
		H[i] = 0;
*/
	for(y = -w; y <= w; y++) {
		for(x = -w; x <= w; x++) {
			H[src->getValue(x, y)]++;
		}
	}

	for(m = 0, n = 0; n < 256; n++) {
		m += H[n];
		if(m >= center)
			break;
	}

	median = n;
	delta  = m - H[n];

	pos[0][0] = median;

	//printf("Point 1\n");

	for(x = 1, y = 0; y < height; y++) {
		A = y-w;
		B = y+w;
		
		//printf("Point 2: %d\n", y);

		for(; x >= 0 && x < width; x += direction) {
			prev = x - direction * (w+1);
			next = x + direction * w;

			//printf("Point 3: %d\n", x);

			for(n = A; n <= B; n++) {
				X = src->getValue(prev, n);
				Y = src->getValue(next, n);
				if(X == Y)
					continue;

				H[X]--;
				if(X < median)
					delta--;
				H[Y]++;
				if(Y < median)
					delta++;
			}

			if(delta >= center) {
				while(delta >= center) {
					if(H[--median] > 0)
						delta -= H[median];
				}
			}
			else {
				while(delta + H[median] < center) {
					if(H[median] > 0)
						delta += H[median];
					median++;
				}
			}

			//printf("Point 4: %d, %d, %d\n", x, y, median);

			pos[x][y] = median;
		}

		if(y == height-1)
			return;

		x -= direction;
		direction *= -1;

		prev = A;
		next = B+1;

		//printf("Point 6\n");

		C = x-w;
		D = x+w;

		for(n = C; n <= D; n++) {
			X = src->getValue(n, prev);
			Y = src->getValue(n, next);

			if(X == Y)
				continue;

			H[X]--;
			if(X < median)
				delta--;
			H[Y]++;
			if(y < median)
				delta++;
		}

		if(delta >= center) {
			while(delta >= center) {
				if(H[--median] > 0)
					delta -= H[median];
			}
		}
		else {
			while(delta + H[median] < center) {
				if(H[median] > 0)
					delta += H[median];
				median++;
			}
		}

		pos[x][y+1] = median;
		x += direction;
	}

	delete src;
}

void CImage::median2(int w)
{
	CImage* src = new CImage(this);
	int X = 0;
	int Y = 0;
	int center = (((2*w+1)*2)-1)/2;
	std::list<int> l;

	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			l.clear();

			for(int v = -w; v <= w; ++v) {
				for(int u = -w; u <= w; ++u) {
					X = x + u;
					Y = y + v;

					l.push_back(src->getValue(X, Y));
				}
			}
			l.sort();

			for(int i = 0; i < center; ++i)
				l.pop_front();
			pos[x][y] = *l.begin();
		}
	}	

	delete src;
}

void CImage::calcHistogram()
{
	memset(histogram, 0, sizeof(histogram));
	
	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			histogram[pos[x][y]]++;
		}
	}
}

void CImage::addLight(float l)
{
	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			pos[x][y] = (int)((float)(pos[x][y])*l);
		}
	}
}

void CImage::invert()
{
	int temp = 0;
	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			temp = pos[x][y];
			if(temp > 255) temp = 255;
			else if(temp < 0) temp = 0;
	
			pos[x][y] = 255 - temp;	
		}
	}
}

void CImage::globalThreshold(int t)
{
	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			if(pos[x][y] < t)
				pos[x][y] = 0;
			else
				pos[x][y] = 255;
		}
	}
}

void CImage::setBorder(int w)
{
	int tempBorder = w;
	if(tempBorder > width) tempBorder = width;

	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < tempBorder; ++x) {
			pos[x][y] = 0;
			pos[width-1-x][y] = 0;
		}
	}

	tempBorder = w;
	if(tempBorder > height) tempBorder = height;

	for(int x = 0; x < width; ++x) {
		for(int y = 0; y < tempBorder; ++y) {
			pos[x][y] = 0;
			pos[x][height-1-y] = 0;
		}
	}
}

int CImage::floodFill(int x, int y, int o, int n)
{
	int iNumPixel = 0;

	if((pos[x][y] == o) && (x >= 0) && (y >= 0) && (x < width) && (y < height)) {
		pos[x][y] = n;
		iNumPixel++;

		//printf("%d %d\n", x, y);

		iNumPixel += floodFill(x+1, y, o, n);
		iNumPixel += floodFill(x-1, y, o, n);
		iNumPixel += floodFill(x, y+1, o, n);
		iNumPixel += floodFill(x, y-1, o, n);
	}

	return iNumPixel;
}

int CImage::flood4Fill(int x, int y, int o, int n)
{
	int iNumPixel = 0;

	std::vector<std::pair<int, int> > stack;
	std::pair<int, int> vOperator;

	stack.push_back(std::make_pair(x, y));

	while(!stack.empty()) {
		vOperator = stack.back();
		stack.pop_back();

		x = vOperator.first;
		y = vOperator.second;

		if(pos[x][y] == o) {
			pos[x][y] = n;
			iNumPixel++;

			stack.push_back(std::make_pair(x, y + 1));
			stack.push_back(std::make_pair(x, y - 1));
			stack.push_back(std::make_pair(x + 1, y));
			stack.push_back(std::make_pair(x - 1, y));
		}	
	}

	return iNumPixel;
}

void CImage::sobel()
{
	CImage* src = new CImage(this);
	float g1 = 0;
	float g2 = 0;

	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			g1 = 0;
			g1 += src->getValue(x-1, y+1);
			g1 += src->getValue(x-1, y) * 2;
			g1 += src->getValue(x-1, y-1);
			g1 += src->getValue(x+1, y+1) * -1;
			g1 += src->getValue(x+1, y) * -2;
			g1 += src->getValue(x+1, y-1) * -1;

			g2 = 0;
			g2 += src->getValue(x-1, y+1);
			g2 += src->getValue(x, y+1) * 2;
			g2 += src->getValue(x+1, y+1);
			g2 += src->getValue(x-1, y-1) * -1;
			g2 += src->getValue(x, y-1) * -2;
			g2 += src->getValue(x+1, y-1) * -1;

			pos[x][y] = sqrt((g1*g1)+(g2*g2));
		}
	}
	
	delete src;
}

void CImage::scharr()
{
	CImage* src = new CImage(this);
	float g1 = 0;
	float g2 = 0;

	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			g1 = 0;
			g1 += src->getValue(x-1, y+1) * 3;
			g1 += src->getValue(x-1, y) 	* 10;
			g1 += src->getValue(x-1, y-1) * 3;
			g1 += src->getValue(x+1, y+1) * -3;
			g1 += src->getValue(x+1, y) 	* -10;
			g1 += src->getValue(x+1, y-1) * -3;

			g2 = 0;
			g2 += src->getValue(x-1, y+1) * 3;
			g2 += src->getValue(x, y+1) 	* 10;
			g2 += src->getValue(x+1, y+1) * 3;
			g2 += src->getValue(x-1, y-1) * -3;
			g2 += src->getValue(x, y-1) 	* -10;
			g2 += src->getValue(x+1, y-1) * -3;

			pos[x][y] = sqrt((g1*g1)+(g2*g2));
		}
	}
	
	delete src;
}

void CImage::kirsch()
{
	CImage* src = new CImage(this);
	float g[8];
	int k[8][9];
	int X, Y, l;

	k[0][0] =  5; k[0][1] =  5; k[0][2] =  5;
	k[0][3] = -3; k[0][4] =  0; k[0][5] = -3;
	k[0][6] = -3; k[0][7] = -3; k[0][8] = -3;

	k[1][0] =  5; k[1][1] =  5; k[1][2] = -3;
	k[1][3] =  5; k[1][4] =  0; k[1][5] = -3;
	k[1][6] = -3; k[1][7] = -3; k[1][8] = -3;

	k[2][0] =  5; k[2][1] = -3; k[2][2] = -3;
	k[2][3] =  5; k[2][4] =  0; k[2][5] = -3;
	k[2][6] =  5; k[2][7] = -3; k[2][8] = -3;

	k[3][0] = -3; k[3][1] = -3; k[3][2] = -3;
	k[3][3] =  5; k[3][4] =  0; k[3][5] = -3;
	k[3][6] =  5; k[3][7] =  5; k[3][8] = -3;

	k[4][0] = -3; k[4][1] = -3; k[4][2] = -3;
	k[4][3] = -3; k[4][4] =  0; k[4][5] = -3;
	k[4][6] =  5; k[4][7] =  5; k[4][8] =  5;

	k[5][0] = -3; k[5][1] = -3; k[5][2] = -3;
	k[5][3] = -3; k[5][4] =  0; k[5][5] =  5;
	k[5][6] = -3; k[5][7] =  5; k[5][8] =  5;

	k[6][0] = -3; k[6][1] = -3; k[6][2] =  5;
	k[6][3] = -3; k[6][4] =  0; k[6][5] =  5;
	k[6][6] = -3; k[6][7] = -3; k[6][8] =  5;

	k[7][0] = -3; k[7][1] =  5; k[7][2] =  5;
	k[7][3] = -3; k[7][4] =  0; k[7][5] =  5;
	k[7][6] = -3; k[7][7] = -3; k[7][8] = -3;
	
	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			
			for(int i = 0; i < 8; ++i) {
				g[i] = 0;
				l = 0;
	
				for(int v = -1; v <= 1; ++v) {
					for(int u = -1; u <= 1; ++u) {
						X = x+u;
						Y = y+v;

						g[i] += src->getValue(X, Y) * k[i][l];

						l++;
					}
				}
			}

			pos[x][y] = *std::max_element(g, g+8);
		}
	}
	
	delete src;
}

void CImage::roberts()
{
	CImage* src = new CImage(this);
	float b1 = 0;
	float b2 = 0;

	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			b1 = 0;
			b1 += src->getValue(x, y);
			b1 -= src->getValue(x+1, y+1);
		
			b2 = 0;
			b2 += src->getValue(x+1, y);
			b2 -= src->getValue(x, y+1);
		
			pos[x][y] = abs(b1) + abs(b2);
		}
	}
	
	delete src;
}

std::vector<SCrack2> CImage::calcBoundary(int x, int y, int f)
{
	std::vector<SCrack2> result;
	CImage* src = new CImage(this);
	SVector2 start;
	start.x = 0;
	start.y = 0;
	
	for(int i = x; i < src->width-1; ++i) {
		if((src->pos[i][y] == f) && (src->pos[i+1][y] != f)) {
			start.x = i;
			start.y = y;
			break;
		}
	}

	start.x = 2 + 2 * start.x;
	start.y = 2 * start.y;

	//printf("%d, %d\n", start.x, start.y);

	if(start.y == 0 && start.x == 0)
		return result;

	SVector2 P = start;
	SVector2 POS;
	SVector2 NEG;
	SVector2 Crack;
	int iDirection = 1;

	int cnt = 0;
	
	do {
		SCrack2 sCrack2;

		if(iDirection == 0) {
			POS.x = P.x+1; POS.y = P.y+1;
			NEG.x = P.x+1; NEG.y = P.y-1;
		}
		else if(iDirection == 1) {
			POS.x = P.x-1; POS.y = P.y+1;
			NEG.x = P.x+1; NEG.y = P.y+1;
		}
		else if(iDirection == 2) {
			POS.x = P.x-1; POS.y = P.y-1;
			NEG.x = P.x-1; NEG.y = P.y+1;
		}
		else {
			POS.x = P.x+1; POS.y = P.y-1;
			NEG.x = P.x-1; NEG.y = P.y-1;
		}

		int ix = (int)(POS.x/2.0f);
		int iy = (int)(POS.y/2.0f);

		if(src->pos[ix][iy] != f)
			iDirection = (iDirection+1)%4;
		else {
			ix = (int)(NEG.x/2.0f);
			iy = (int)(NEG.y/2.0f);
			
			if(src->pos[ix][iy] == f)
				iDirection = (iDirection+3)%4;
		}

		Crack = P;

		if(iDirection == 0) Crack.x += 1;
		else if(iDirection == 1) Crack.y += 1;
		else if(iDirection == 2) Crack.x -= 1;
		else Crack.y -= 1;

		sCrack2.x = Crack.x;
		sCrack2.y = Crack.y;
		sCrack2.d = iDirection;
		sCrack2.u = P.x;
		sCrack2.v = P.y;

		result.push_back(sCrack2);

	//	printf("Crack %d: %d, %d, %d\n", cnt, Crack.x, Crack.y, iDirection);
		cnt++;
		
		if(iDirection == 0)
			P.x += 2;
		else if(iDirection == 1)
			P.y += 2;
		else if(iDirection == 2)
			P.x -= 2;
		else 
			P.y -= 2;
	} while((P.x != start.x) || (P.y != start.y));

//	printf("num cracks: %d\n", cnt);

	delete src;
	
	return result;
}

int CImage::getH(SVector2 v, DSSData* p)
{
	return p->a * (v.x - p->StartP.x) + p->b * (v.y - p->StartP.y);
}

int CImage::recoDSS(SCrack2 crack, DSSData* p)
{
	SVector2 POS;
	SVector2 NEG;

	if(crack.d == 0) {
		POS.x = crack.x; POS.y = crack.y+1;
		NEG.x = crack.x; NEG.y = crack.y-1;
	}
	else if(crack.d == 1) {
		POS.x = crack.x-1; POS.y = crack.y;
		NEG.x = crack.x+1; NEG.y = crack.y;
	}
	else if(crack.d == 2) {
		POS.x = crack.x; POS.y = crack.y-1;
		NEG.x = crack.x; NEG.y = crack.y+1;
	}
	else {
		POS.x = crack.x+1; POS.y = crack.y;
		NEG.x = crack.x-1; NEG.y = crack.y;
	}

	if(p->CC == 0) {
		p->iProhib1 = (crack.d + 2) % 4;
		p->iProhib2 = -1;
		p->CC = 1;

		p->StartP = p->EndP = POS;
		p->StartN = p->EndN = NEG;
		
		if(crack.d == 0) {
			p->a = 0;
			p->b = 1;
		}
		else if(crack.d == 1) {
			p->a = -1;
			p->b = 0;
		}
		else if(crack.d == 2) {
			p->a = 0;
			p->b = -1;
		}
		else {
			p->a = 1;
			p->b = 0;
		}
	}
	if(crack.d == p->iProhib1 || crack.d== p->iProhib2) 
		return 1;
	if(crack.d != ((p->iProhib1 + 2) % 4) && p->iProhib2 == -1)
		p->iProhib2 = (crack.d + 2) % 4;
	if(p->CC == 1) {
		p->EndP = POS;
		p->EndN = NEG;
		p->CC = 2;
		
		if(p->iProhib2 == -1)
			return 0;
		if(p->EndP.x == p->StartP.x && p->EndP.y == p->StartP.y) {
      	p->a = -(p->EndN.y - p->StartN.y) / p->e;
			p->b = (p->EndN.x - p->StartN.x) / p->e; 
		}
     	else {
			p->a = -(p->EndP.y - p->StartP.y) / p->e;
			p->b = (p->EndP.x - p->StartP.x) / p->e;
		}
	
      return 0;
	}

	int HP = getH(POS, p);
	int HN = getH(NEG, p);
	
	if(HP < -p->e || HN > 0) 
		return HP;

	if(HP == 0)
		p->EndP = POS;
	else {
      if(HP == -p->e) {
			p->EndP = POS;
			p->StartN = p->EndN;
			p->a = -(p->EndP.y - p->StartP.y) / p->e;
			p->b = (p->EndP.x - p->StartP.x) / p->e;
      }
	}

	if(HN == -p->e) 
		p->EndN = NEG;
	else {
		if(HN == 0) {
			p->EndN = NEG;
			p->StartP = p->EndP;
         p->a = -(p->EndN.y - p->StartN.y) / p->e;
			p->b = (p->EndN.x - p->StartN.x) / p->e;
       }
	}

	return 0;
}

std::vector<SVector2> CImage::createDSS(std::vector<SCrack2> boundary)
{
	std::vector<SVector2> result;
	DSSData data;
	data.e = 1;
	data.CC = 0;
	data.iProhib1 = -1;
	data.iProhib2 = -1;
	int state = -1;

	SVector2 vertex;
	vertex.x = boundary[0].u;
	vertex.y = boundary[0].v;
	result.push_back(vertex);

	int endLoop = boundary.size();
	for(int i = 0; i < endLoop; ++i) {
		state = recoDSS(boundary[i], &data);

		if(state != 0) {
			data.CC = 0;
			data.iProhib1 = -1;
			data.iProhib2 = -1;
			
			vertex.x = boundary[i].u;
			vertex.y = boundary[i].v;
			result.push_back(vertex);
		}
	}
	

	return result;
}

float CImage::getLengthFromDSS(std::vector<SVector2> v)
{
	float result = 0.0f;
	
	int size = v.size()-1;
	for(int i = 0; i < size; ++i)
		result += sqrt((pow((float)(v[i+1].x - v[i].x), 2) + pow((float)(v[i+1].y - v[i].y), 2)));
	result += sqrt((pow((float)(v[0].x - v[size].x), 2) + pow((float)(v[0].y - v[size].y), 2)));

	return result;
}

std::pair<SVector2, SVector2> CImage::getOBBFromShard(std::vector<SCrack2> v)
{
	SVector2 min;
	SVector2 max;
	
	min.x = INT_MAX;
	min.y = INT_MAX;
	max.x = INT_MIN;
	max.y = INT_MIN;

	int Size = v.size();
	for(int i = 0; i < Size; ++i) {
		if(v[i].u < min.x)
			min.x = v[i].u;
		if(v[i].v < min.y)
			min.y = v[i].v;
		if(v[i].u > max.x)
			max.x = v[i].u;
		if(v[i].v > max.y)
			max.y = v[i].v;
	}

	return std::make_pair(min, max);
}

QImage CImage::flood4Fill(QImage* img, int x, int y, QColor o, QColor n)
{
	std::vector<std::pair<int, int> > stack;
	std::pair<int, int> vOperator;

	if(o != n )
		stack.push_back(std::make_pair(x, y));

	while(!stack.empty()) {
		vOperator = stack.back();
		stack.pop_back();

		x = vOperator.first;
		y = vOperator.second;

		if(QColor(img->pixel(x, y)) == o) {
			img->setPixel(x, y, qRgb(n.red(), n.green(), n.blue()));

			stack.push_back(std::make_pair(x, y + 1));
			stack.push_back(std::make_pair(x, y - 1));
			stack.push_back(std::make_pair(x + 1, y));
			stack.push_back(std::make_pair(x - 1, y));
		}	
	}

	return *img;
}

void CImage::globalContrast(int min, int max)
{
	int iGMin = -1;
	int iGMax = -1;
	int iWMin = min;
	int iWMax = max;

	calcHistogram();

	for(int i = 0; i < 256; ++i) {
		if(iGMin == -1) {
			if(histogram[i] > 0) {
				iGMin = i;
				break;
			}
		}
	}

	for(int i = 255; i >= 0; --i) {
		if(iGMax == -1) {
			if(histogram[i] > 0) {
				iGMax = i;
				break;
			}
		}
	}

	int konst = (int)((float)(iWMax - iWMin) / (float)(iGMax - iGMin));

	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			pos[x][y] = (pos[x][y] - iGMin) * konst;
		}
	}
}

void CImage::gammaPatch(int max, int gamma) 
{
	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			pos[x][y] = max * (pow((float)(pos[x][y]/max), gamma));
		}
	}
}

SSegmentInfo CImage::seedRegionGrowing(int x, int y, int t, int c)
{
	SSegmentInfo ssinfo;
	ssinfo.min.x = -1;
	ssinfo.min.y = -1;
	ssinfo.max.x = -1;
	ssinfo.max.y = -1;
	int iNumPixel = 0;

	std::vector<std::pair<int, int> > stack;
	std::pair<int, int> vOperator;

	int seedColor = pos[x][y];

	int markedColor = -1;

	stack.push_back(std::make_pair(x, y));

	while(!stack.empty()) {
		vOperator = stack.back();
		stack.pop_back();

		x = vOperator.first;
		y = vOperator.second;

		if(qualitySqare(seedColor, pos[x][y]) < t) {
			pos[x][y] = markedColor;
			
			if(ssinfo.max.x == -1) {
				ssinfo.min.x = x;
				ssinfo.min.y = y;
				ssinfo.max.x = x;
				ssinfo.max.y = y;
			} else {
				if(x > ssinfo.max.x) ssinfo.max.x = x;
				if(y > ssinfo.max.y) ssinfo.max.y = y;
				if(x < ssinfo.min.x) ssinfo.min.x = x;
				if(y < ssinfo.min.y) ssinfo.min.x = y;
			}

			iNumPixel++;

			if(pixelConditions(x, y+1, markedColor))
				stack.push_back(std::make_pair(x, y + 1));
			if(pixelConditions(x, y-1, markedColor))
				stack.push_back(std::make_pair(x, y - 1));
			if(pixelConditions(x+1, y, markedColor))
				stack.push_back(std::make_pair(x + 1, y));
			if(pixelConditions(x-1, y, markedColor))
				stack.push_back(std::make_pair(x - 1, y));
		}
	}

	if(c >= 0) {
		for(int y = 0; y < height; ++y) {
			for(int x = 0; x < width; ++x) {
				if(pos[x][y] == markedColor) {
					pos[x][y] = c;
				}
			}
		}
	}

	ssinfo.num_pixel = iNumPixel;
	return ssinfo;
}

float CImage::qualitySqare(int a, int b)
{
	return (float)(pow((float)(a - b), 2));
}

float CImage::qualityAbs(int a, int b)
{
	return (float)(abs(a-b));
}

bool CImage::pixelConditions(int x, int y, int m)
{
	if((x > 0) && (y > 0) && (x < width) && (y < height))
		if(pos[x][y] != m)
			return true;
	return false;
}

void CImage::regionGrowing(int l, int t, int c)
{
	int Height 	= height-1;
	int Width 	= width-1;
	int seedColor = 0;

	for(int y = 1; y < Height; ++y) {
		for(int x = 1; x < Width; ++x) {
			seedColor = pos[x][y];
			if(seedColor > l) {
				
				if(pixelConditions(x, y+1, -1))
					if(qualityAbs(seedColor, pos[x][y+1]) >= t)
						continue;
				if(pixelConditions(x, y-1, -1))
					if(qualityAbs(seedColor, pos[x][y-1]) >= t)
						continue;
				if(pixelConditions(x+1, y, -1))
					if(qualityAbs(seedColor, pos[x+1][y]) >= t)
						continue;
				if(pixelConditions(x-1, y, -1))
					if(qualityAbs(seedColor, pos[x-1][y]) >= t)
						continue;
			
				seedRegionGrowing(x, y, t, -1);
			}
		}
	}

	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			if(pos[x][y] == -1)
				pos[x][y] = c;
		}
	}
}
