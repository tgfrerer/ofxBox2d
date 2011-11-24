/*
 *     _____    ___     
 *    /    /   /  /     ofxBox2dChainShape
 *   /  __/ * /  /__    (c) ponies & light, 2011. 
 *  /__/     /_____/    oo43.com
 *
 *  ofxBox2dChainShape.cpp
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

#include "ofxBox2dChainShape.h"

// ----------------------------------------------------------------------

ofxBox2dChainShape::ofxBox2dChainShape()
:bIsSimplified(FALSE)
{ 

}

// ----------------------------------------------------------------------

ofxBox2dChainShape::~ofxBox2dChainShape() { 
}

// ----------------------------------------------------------------------

void ofxBox2dChainShape::destroy() {
	ofxBox2dBaseShape::destroy();
	clear();
}

// ----------------------------------------------------------------------

void ofxBox2dChainShape::setup(b2World * b2dworld) {
	
}

// ----------------------------------------------------------------------

void ofxBox2dChainShape::updateShape() {
	
	bounds = getBoundingBox();
}

// ----------------------------------------------------------------------

void ofxBox2dChainShape::addVertexes(vector <ofVec2f> &pts) {
	for (int i=0; i<pts.size(); i++) {
		addVertex(pts[i].x, pts[i].y);
	}
}

//----------------------------------------
void ofxBox2dChainShape::addVertexes(ofPolyline &polyline) {
	for (int i=0; i<polyline.size(); i++) {
		addVertex(polyline[i].x, polyline[i].y);
	}
}

//----------------------------------------
void ofxBox2dChainShape::create(b2World * b2dworld) {
	
	if(size() < 3) {
		printf("need at least 3 points\n");
		return;	
	}
	
	if (body != NULL) {
		b2dworld->DestroyBody(body);
		body = NULL;
	}
	
	// create the body from the world (1)
	b2BodyDef		bd;
	bd.type			= density <= 0.0 ? b2_staticBody : b2_dynamicBody;
	body			= b2dworld->CreateBody(&bd);
	
	vector<b2Vec2> verts;
	for (int i=0; i<size(); i++) {
		ofVec2f p = getVertices()[i] / OFX_BOX2D_SCALE;
		verts.push_back(b2Vec2(p.x, p.y));
	}
	b2ChainShape	shape;
	shape.CreateChain(&verts[0], verts.size());
	
	fixture.shape		= &shape;
	fixture.density		= density;
	fixture.restitution = bounce;
	fixture.friction	= friction;	
	
	body->CreateFixture(&fixture);
	
	// update the area and centroid
	updateShape();
}

//------------------------------------------------
void ofxBox2dChainShape::addAttractionPoint (ofVec2f pt, float amt) {
    // we apply forces at each vertex. 
    if(body != NULL) {
        const b2Transform& xf = body->GetTransform();
		
        for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
            b2ChainShape* shape = (b2ChainShape*)f->GetShape();
            
            if(shape) {
                b2Vec2 P(pt.x/OFX_BOX2D_SCALE, pt.y/OFX_BOX2D_SCALE);
                
                for(int i=0; i<shape->m_count; i++) {
                    b2Vec2 qt = b2Mul(xf, shape->m_vertices[i]);
                    b2Vec2 D = P - qt; 
                    b2Vec2 F = amt * D;
                    body->ApplyForce(F, P);
                }                    
            }
        }
    }
}


//----------------------------------------
void ofxBox2dChainShape::simplify(float tolerance) {
	ofPolyline::simplify(tolerance);	
	bIsSimplified = true;
}


//----------------------------------------
void ofxBox2dChainShape::addAttractionPoint (float x, float y, float amt) {
    addAttractionPoint(ofVec2f(x, y), amt);
}

//----------------------------------------
void ofxBox2dChainShape::addRepulsionForce(float x, float y, float amt) {
	addRepulsionForce(ofVec2f(x, y), amt);
}
void ofxBox2dChainShape::addRepulsionForce(ofVec2f pt, float amt) {
	// we apply forces at each vertex. 
    if(body != NULL) {
        const b2Transform& xf = body->GetTransform();
		
        for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
            b2ChainShape* shape = (b2ChainShape*)f->GetShape();
            
            if(shape) {
                b2Vec2 P(pt.x/OFX_BOX2D_SCALE, pt.y/OFX_BOX2D_SCALE);
                
                for(int i=0; i<shape->m_count; i++) {
                    b2Vec2 qt = b2Mul(xf, shape->m_vertices[i]);
                    b2Vec2 D = P - qt; 
                    b2Vec2 F = amt * D;
                    body->ApplyForce(-F, P);
                }                    
            }
        }
    }
}

// ----------------------------------------------------------------------

void ofxBox2dChainShape::draw() {
	
	//ofNoFill();
	//ofRect(bounds);
	//ofVec2f centroid = getCenter();
	//ofCircle(centroid.x, centroid.y, 2);
	
	if(body == NULL) {
		return;	
	}
	
	const b2Transform& xf = body->GetTransform();
	
	for (b2Fixture * f = body->GetFixtureList(); f; f = f->GetNext()) {
		b2ChainShape * shape = (b2ChainShape*)f->GetShape();
		
		if(shape) {
			drawShape.clear();
			for(int i=0; i<shape->m_count; i++) {
				b2Vec2 pt = b2Mul(xf, shape->m_vertices[i]);
				drawShape.addVertex(pt.x*OFX_BOX2D_SCALE, pt.y*OFX_BOX2D_SCALE);
			}
			if(isClosed()) drawShape.close();
			drawShape.draw();
			
		}
	}
	
	
	
}














