#include "Includes.h"

typedef enum{D3_R, D3_L, D3_D, D3_U, D3_F, D3_B}Direction3;

static inline
bool isD3RL(const Direction3 d3)
{
	return d3 == D3_R || d3 == D3_L;
}

static inline
bool isD3DU(const Direction3 d3)
{
	return d3 == D3_D || d3 == D3_U;
}

static inline
bool isD3FB(const Direction3 d3)
{
	return d3 == D3_F || d3 == D3_B;
}

static inline
bool isD3Neg(const Direction3 d3)
{
	return d3 & 0b1;
}

static inline
bool isD3Pos(const Direction3 d3)
{
	return !isD3Neg(d3);
}

static inline
Direction3 D3Inv(const Direction3 d3)
{
	if(isD3RL(d3))
		return d3 == D3_R ? D3_L : D3_R;
	if(isD3DU(d3))
		return d3 == D3_D ? D3_U : D3_D;
	return d3 == D3_F ? D3_B : D3_F;
}

typedef union{
	int arr[3];
	struct{
		int x;
		int y;
		int z;
	};
}Coord3;

Coord3 coord3Shift(const Coord3 c3, const Direction3 d3, const int units)
{
	Coord3 ret = c3;
	if(isD3RL(d3))
		ret.x += units;
	else if(isD3DU(d3))
		ret.y += units;
	else
		ret.z += units;
	return ret;
}

Coord coord3Project(const Coord3 c3, const Range win, const Offset zscale)
{
	Coord ret = {win.x/2 + c3.x, win.y/2 + c3.y};
	ret.x += c3.z*zscale.x;
	ret.y += c3.z*zscale.y;
	return ret;
}

void drawAll(const Range win, const int scale, const Offset zscale)
{
	for(int x = -4; x <= 4; x++){
		for(int y = -4; y <= 4; y++){
			for(int z = -4; z <= 4; z++){
				Coord3 c3 = {x*scale, y*scale, z*scale};
				Coord c2 = coord3Project(c3, win, zscale);
				drawPixelCoord(c2);
			}
		}
	}
}

int main(int argc, char const *argv[])
{
	const Range win = {1000, 1000};
	const int scale = 10;
	Offset zscale = {3, -3};
	init(win);

	while(1){
		Ticks frameStart = getTicks();
		clear();
		setColor(WHITE);
		drawAll(win, scale, zscale);
		draw();
		events(frameStart + TPF, &zscale);
	}
	return 0;
}
