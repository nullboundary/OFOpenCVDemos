#ifndef _VEC_FIELD
#define _VEC_FIELD


#include "ofMain.h"


#define NUM_BINS_X 80
#define NUM_BINS_Y 60

class vectorField{

	public:
		vectorField(){
			binW = 1.0/NUM_BINS_X;
			binH = 1.0/NUM_BINS_Y;
		}

		//-------------------------------------------------
		void addIntoField(float x, float y, ofPoint vec, float radius = 1.0){

			if(x >= 1.0 || x < 0 || y >= 1.0 || y < 0){
				 return;
			}

			ofPoint pos(x,y);
			ofPoint binPos;
			ofPoint distVec;
			float dist = 0;
			float amnt = 0;

			for(int yy = 0; yy < NUM_BINS_Y; yy++){
				for(int xx = 0; xx < NUM_BINS_X; xx++){

					binPos.set( (float)xx * binW, (float)yy * binH);
					distVec = binPos - pos;
					dist = distVec.length();

					if(dist > radius)continue;

					amnt = 1.0 - dist;

					//amnt * amnt makes the rolloff less linear
					field[xx][yy] += (amnt * amnt * amnt) * vec;
				}
			}
		}

	//-------------------------------------------------
	void normalizeDirection(ofPoint vec){

		for(int yy = 0; yy < NUM_BINS_Y; yy++){
			for(int xx = 0; xx < NUM_BINS_X; xx++){
				field[xx][yy].set(vec);
			}
		}

	}

	void addToFieldVectorByIndex(int xIndex, int yIndex,ofPoint vec)
	{

	   // ofPoint dVec;
            ofPoint cVec = field[xIndex][yIndex];
	  //  dVec = vec - cVec;
	  //  float dist = dVec.length();
	  //  if(dist < 0.5)
	  //  {
	         cVec = cVec + vec ;

            cVec.x = ofClamp(cVec.x,-0.9,0.9);
            cVec.y = ofClamp(cVec.y,-0.9,0.9);
            //if(dist < 0.9)

            field[xIndex][yIndex].set(cVec);
            //field[xIndex][yIndex] += cVec;
	  //  }

	    //field[xIndex][yIndex] += cVec;
	}


	void setFieldVectorByIndex(int xIndex, int yIndex,ofPoint vec)
	{

		field[xIndex][yIndex].set(vec);
	}

		//-------------------------------------------------
		void blur(float amnt){

			if(amnt > 1.0)amnt = 1.0;
			else if(amnt < 0.0)amnt = 0.0;

			ofPoint blur;

			float ramnt = (1.0 - amnt) * 0.25;

			for(int yy = 1; yy < NUM_BINS_Y-1; yy++){
				for(int xx = 1; xx < NUM_BINS_X-1; xx++){

					blur = field[xx][yy] * amnt + field[xx+1][yy] * ramnt +  field[xx-1][yy] * ramnt + field[xx][yy+1] * ramnt + field[xx][yy-1] * ramnt;
					field[xx][yy] = blur;
				}
			}
		}


		//-------------------------------------------------
		ofPoint readFromField(float x, float y){

			if(x >= 1.0 || x < 0 || y >= 1.0 || y < 0){
				 return ofPoint();
			}

			int rasterX = x * NUM_BINS_X;
			int rasterY = y * NUM_BINS_Y;

			ofPoint vec = field[rasterX][rasterY];

			return vec;
		}


		//-------------------------------------------------
		void normalize(){

			for(int yy = 0; yy < NUM_BINS_Y; yy++){
				for(int xx = 0; xx < NUM_BINS_X; xx++){
					field[xx][yy].normalize();
				}
			}
		}

		//-------------------------------------------------
		void clear(){

			for(int yy = 0; yy < NUM_BINS_Y; yy++){
				for(int xx = 0; xx < NUM_BINS_X; xx++){
					field[xx][yy].set(0,0);
				}
			}
		}

		//-------------------------------------------------
		void draw(int x, int y, float width, float height, float scale = 1.0){

			ofPoint pixelPos;
			ofPoint vec;

			glPushMatrix();
				glTranslatef(x, y, 0);

				for(int yy = 0; yy < NUM_BINS_Y; yy++){
					for(int xx = 0; xx < NUM_BINS_X; xx++){

						pixelPos.set( (float)xx * binW * width, (float)yy * binH * height );
						vec = (field[xx][yy]) * scale;

						ofLine(pixelPos.x, pixelPos.y, pixelPos.x + vec.x, pixelPos.y + vec.y);
					}
				}

			glPopMatrix();
		}


	ofPoint field[NUM_BINS_X][NUM_BINS_Y];
	float binW, binH;


};

#endif

