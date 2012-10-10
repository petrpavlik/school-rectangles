//
//  main.cpp
//  POO
//
//  Created by Petr Pavlik on 10/3/12.
//  Copyright (c) 2012 Petr Pavlik. All rights reserved.
//

#include <iostream>

unsigned int rectWidth = 10;
unsigned int rectHeight = 12;
int bestResult = -1;

long long steps = 0;

struct Rect
{
	/// Constructor.
	Rect(int x, int y, int width, int height) : x(x), y(y), width(width), height(height)
	{
	}
    
    Rect(int width, int height) : x(0), y(0), width(width), height(height)
	{
	}

	int right() const
	{
		return x + width - 1;
	}
	
	int bottom() const
	{
		return y + height - 1;
	}
	
	int x; ///< X coordinate of the top-left corner.
	int y; ///< Y coordinate of the top-left corner.
	int width; ///< Width of the rectangle.
	int height; ///< Height of the rectangle.
};


struct Field
{
    Field(unsigned int width, unsigned int height) {

        this->width = width;
        this->height = height;
        
        array = new char[width*height];
        memset(array, 0, width*height*sizeof(char));
    }
    
    ~Field() {
        
        delete[] array;
    }
    
    Field(const Field& other) {
        width = other.width;
        height = other.height;
        array = new char[width*height];
        memcpy(array, other.array, width*height*sizeof(char));
    }
    
    bool addRect(Rect rect) {
        
        //How many gaps were skipped. Is this number greater than bestResult?
        int numSkippedGaps = 0;
     
        for (int y=0; y<rectHeight; y++) {
            
            for (int x=0; x<rectWidth; x++) {
                
                //find first free cell in this row
                if (arrayValueAtPosition(x, y)==0) {
                    
                    rect.x = x;
                    rect.y = y;
                    
                    //can the rect fit here?
                    if (tryToFitRect(rect)) {
                        return true;
                    }
                    
                    numSkippedGaps++;
                    
                    if (bestResult!=-1 && numSkippedGaps>=bestResult) {
                        //we can cut this off
                        std::cout << "cutting" << std::endl;
                        return false;
                    }
                }
            }
        }
        
        //cannot fit it anywhere
        if (containsRequiredRects()) {
            //print();
            
            int result = getNumberOfGaps();
            
            if (bestResult == -1 || result < bestResult) {
                bestResult = result;
            }
        }
        return false;
    }
    
    bool tryToFitRect(Rect rect) {
        
        //don't waste time if the rect exceeds boundaries of the field
        if (rect.right()>=width || rect.bottom()>=height) {
            return false;
        }
        
        //try to find a taken cell
        for (int y=rect.y; y<=rect.bottom(); y++) {
            
            for (int x=rect.x; x<=rect.right(); x++) {
                
                //have we found a taken cell?
                if (arrayValueAtPosition(x, y)!=0) {
                    return false; //then we cannot fit it
                }
            }
        }
        
        //fit the rect
        for (int y=rect.y; y<=rect.bottom(); y++) {
            
            for (int x=rect.x; x<=rect.right(); x++) {
                setArrayValueAtPosition(x, y, rect.width*10+rect.height);
            }
        }
        
        return true;
    }
    
    char arrayValueAtPosition(int x, int y) {
        return array[y*width+x];
    }
    
    void setArrayValueAtPosition(int x, int y, char value) {
        array[y*width+x] = value;
    }
    
    void print() {
        
        for (int y=0; y<height; y++) {
            
            for (int x=0; x<width; x++) {
                
                std::cout << (int)arrayValueAtPosition(x, y) << "\t";
            }
            
            std::cout << std::endl;
        }
        
        std::cout << std::endl;

    }
    
    bool containsRequiredRects() {
        
        bool has3x3 = false;
        bool has2x4 = false;
        bool has4x2 = false;
        bool has1x5 = false;
        bool has5x1 = false;
        
        for (int y=0; y<height; y++) {
            
            for (int x=0; x<width; x++) {
                
                if (arrayValueAtPosition(x, y) == 33) {
                    has3x3 = true;
                }
                else if (arrayValueAtPosition(x, y) == 24) {
                    has2x4 = true;
                }
                else if (arrayValueAtPosition(x, y) == 42) {
                    has4x2 = true;
                }
                else if (arrayValueAtPosition(x, y) == 15) {
                    has1x5 = true;
                }
                else if (arrayValueAtPosition(x, y) == 51) {
                    has5x1 = true;
                }
            }
            
        }
        
        if (has3x3 && (has2x4 || has4x2) && (has1x5 || has5x1)) {
            return true;
        }
        
        return false;
    }
    
    int getNumberOfGaps() {
        
        int numGaps = 0;
        
        for (int y=0; y<height; y++) {
            
            for (int x=0; x<width; x++) {
                
                if (arrayValueAtPosition(x, y) == 0) {
                    numGaps++;
                }
            }
            
        }
        
        return numGaps;
    }
    
    unsigned int width;
    unsigned int height;
    
    char* array;
};

void fillFieldWithRect(Field field, Rect rect) {
    
    /*steps++;
    
    if (steps % 1000 == 0) {
        std::cout << steps << std::endl;
    }*/
    
    if (field.addRect(rect)) {
        //std::cout << "yes";
        
        fillFieldWithRect(field, Rect(3, 3));
        fillFieldWithRect(field, Rect(2, 4));
        fillFieldWithRect(field, Rect(4, 2));
        fillFieldWithRect(field, Rect(1, 5));
        fillFieldWithRect(field, Rect(5, 1));
    }
    else {
        //std::cout << "no";
    }
}

int main(int argc, const char * argv[])
{

    Field field = Field(9, 9);
    
    fillFieldWithRect(field, Rect(3, 3));
    fillFieldWithRect(field, Rect(2, 4));
    fillFieldWithRect(field, Rect(4, 2));
    fillFieldWithRect(field, Rect(1, 5));
    fillFieldWithRect(field, Rect(5, 1));
    
    std::cout << "best result " << bestResult << std::endl;
    
    return 0;
}

