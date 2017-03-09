#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>
#include <QColor>
#include <stdio.h>
#include <list>
#include <algorithm>
#include <math.h>

#pragma once

class QColor;

struct SVector2 {
	int x;
	int y;
};

struct SSegmentInfo {
	SVector2 min;
	SVector2 max;
	int		 num_pixel;
};

struct SCrack2 {
	int x;
	int y;
	int d;
	int u;
	int v;
};

struct DSSData {
	int CC;
	SVector2 StartP;
	SVector2 EndP;
	SVector2 StartN;
	SVector2 EndN;
	int iProhib1;
	int iProhib2;
	int a;
	int b;
	int e;
};

struct SShard {
	int x;
	int y;
	int iColor;
	float A;
	float U;
	std::vector<SCrack2> boundary;
	std::vector<SVector2> dss;
	std::pair<SVector2, SVector2> OBB;
};

class CImage {
private:
	int recoDSS(SCrack2 crack, DSSData* p);
	int getH(SVector2 v, DSSData* p);

float qualitySqare(int a, int b);
	float qualityAbs(int a, int b);
	bool  pixelConditions(int x, int y, int m);
public:
	CImage(QImage* img);
	CImage(CImage* img);
	~CImage();

	int   width;
	int   height;
	int** pos;

	int histogram[256];

	void derivateX();
	void derivateY();
	void integrate();

	void sumation(int w);
	void mean(int w);
	void median(int w);
	void addLight(float l);
	void laplacian();
	void laplacian(int w);
	void sobel();
	void scharr();
	void kirsch();
	void roberts();
	void invert();
	void globalThreshold(int t);
	void setBorder(int w);
	void globalContrast(int min = 0, int max = 255);
	void gammaPatch(int max = 255, int gamma = 1);
	
	int  floodFill(int x, int y, int o, int n);
	int  flood4Fill(int x, int y, int o, int n);
	std::vector<SCrack2> calcBoundary(int x, int y, int f);
	std::vector<SVector2> createDSS(std::vector<SCrack2> boundary);
	float getLengthFromDSS(std::vector<SVector2> v);
	std::pair<SVector2, SVector2> getOBBFromShard(std::vector<SCrack2> v);

	SSegmentInfo seedRegionGrowing(int x, int y, int t, int c);
	void regionGrowing(int l, int t, int c);

	static QImage flood4Fill(QImage* img, int x, int y, QColor o, QColor n);

	// experiment
	void median2(int w);
	void laplacian2();

	void calcHistogram();
	int getValue(int x, int y);
	QImage get();

	int mod(int n, int m) { 
		return (n%m+m)%m; 
	}
};

#endif
