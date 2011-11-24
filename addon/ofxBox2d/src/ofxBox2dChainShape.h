#ifndef GUARD_ofxBox2dChainShape
#define GUARD_ofxBox2dChainShape

/*
 *     _____    ___     
 *    /    /   /  /     ofxBox2dChainShape
 *   /  __/ * /  /__    (c) ponies & light, 2011. 
 *  /__/     /_____/    oo43.com
 *
 *  ofxBox2dChainShape.h
 *  Created by Tim Gfrerer on 24/11/2011.
 *  
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */


#pragma once
#include "ofMain.h"
#include "ofxBox2dBaseShape.h"
#include "ofxBox2dPolygonUtils.h"

class ofxBox2dChainShape : public ofxBox2dBaseShape, public ofPolyline {
	
private:
	bool bIsSimplified;
public:
	
	ofPolyline				drawShape;
	ofRectangle				bounds;
	
	//----------------------------------------
	ofxBox2dChainShape();
	~ofxBox2dChainShape();
	void destroy();
	
	void setup(b2World * b2dworld);
	
	//----------------------------------------
	void addTriangle(const ofVec2f &a, const ofVec2f &b, const ofVec2f &c);
	void addVertexes(vector <ofVec2f> &pts);
	void addVertexes(ofPolyline &polyline);
	
	//----------------------------------------
	// Polygon helper functions
	//----------------------------------------
	void simplify(float tolerance=0.3);
	void triangulate(float resampleAmt=20, int nPointsInside=-1);
	
	//------------------------------------------------
	void addAttractionPoint(ofVec2f pt, float amt=1);
	void addAttractionPoint(float x, float y, float amt=1);
	void addRepulsionForce(float x, float y, float amt);
	void addRepulsionForce(ofVec2f pt, float amt);
	
	//----------------------------------------
	void updateShape();
	void create(b2World * b2dworld);
	void draw();
	
};

#endif