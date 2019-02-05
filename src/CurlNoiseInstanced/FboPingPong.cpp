//
//  FboPingPong.cpp
//  emptyExample
//
//  Created by Andreas Müller on 12/08/2013.
//
//

#include "FboPingPong.h"

// ------------------------------------------------------------------------------------
//
void FboPingPong::allocate( int _w, int _h, int _internalformat, ofColor _clearColor )
{
	ofFbo::Settings settings = ofFbo::Settings();
	settings.width  = _w;		
	settings.height = _h;
	settings.useDepth = true;
	settings.internalformat = _internalformat;
	
	allocate( settings, _clearColor );
}

// ------------------------------------------------------------------------------------
//
void FboPingPong::allocate( ofFbo::Settings _settings, ofColor _clearColor )
{
	clearColor = _clearColor;
	
    ofDisableArbTex();
	fbo1.allocate( _settings);
	fbo2.allocate( _settings );
    ofEnableArbTex();
    
    fbo1.createAndAttachTexture(GL_RGBA32F, 1);
    fbo1.createAndAttachTexture(GL_RGBA32F, 2);
    fbo2.createAndAttachTexture(GL_RGBA32F, 1);
    fbo2.createAndAttachTexture(GL_RGBA32F, 2);
	
	sourceBuffer = &fbo1;
	destBuffer = &fbo2;
	
	clearSource();
	clearDest();
}

// ------------------------------------------------------------------------------------
//
void FboPingPong::draw( ofPoint _pos, float _width, bool _drawBack )
{
	float desWidth = _width;
	float desHeight = (source()->getWidth() / source()->getHeight()) * desWidth;
	
	source()->draw(_pos.x, _pos.y, desWidth, desHeight );
	
	if( _drawBack )
	{
		ofVec2f to = _pos + ofVec2f(desWidth, 0);
		dest()->draw(to.x, to.y, desWidth, desHeight );
	}
}

// ------------------------------------------------------------------------------------
//
void FboPingPong::clearBoth()
{
	clearSource();
	clearDest();
}

// ------------------------------------------------------------------------------------
//
void FboPingPong::clearBoth( ofColor _clearColor )
{
	clearSource( _clearColor );
	clearDest( _clearColor );
}

// ------------------------------------------------------------------------------------
//
void FboPingPong::clearSource()
{
	clearSource( clearColor );
}

// ------------------------------------------------------------------------------------
//
void FboPingPong::clearDest()
{
	clearDest( clearColor );
}

// ------------------------------------------------------------------------------------
//
void FboPingPong::clearSource( ofColor _clearColor )
{
	source()->begin();
		ofClear( _clearColor );
	source()->end();
}

// ------------------------------------------------------------------------------------
//
void FboPingPong::clearDest( ofColor _clearColor )
{
	dest()->begin();
		ofClear( _clearColor );
	dest()->end();
}

// ------------------------------------------------------------------------------------
//
void FboPingPong::setClearColor( ofColor _color )
{
	clearColor = _color;
}

// ------------------------------------------------------------------------------------
//
void FboPingPong::swap()
{
	std::swap(sourceBuffer, destBuffer);
}