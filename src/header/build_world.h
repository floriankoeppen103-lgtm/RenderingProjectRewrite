#ifndef BUILD_WORLD_H
#define BUILD_WORLD_H

#include "settings.h"
#include "types.h"
#include "vector_math.h"

inline bool isBlockTransparent(const struct block* mat, int matCount, int blockID){
    for(int i = 0; i< matCount; i++){
        if(mat[i].ID == blockID){return mat[i].isTransparent;}
    }printf("blockID error - isBlockTransparent\n");return true;
}
inline struct block getMaterial(const struct block* mat, int matCount, int blockID){
    int highestRes = 0;
    for(int i = 0; i < (int)(matCount); i++) {
        if(mat[i].ID == blockID && mat[i].PixelResolution > highestRes && mat[i].PixelResolution <= targetResolution)
            highestRes = mat[i].PixelResolution;
    }
    for(int i = 0; i < (int)(matCount); i++) {
        if(mat[i].ID == blockID && mat[i].PixelResolution == highestRes)
            return mat[i];
    }
    printf("blockID error - getMaterial\n");return {0};
}

// Fills triangle[] from B, returns populatedTriangleCount.
// Writes triangles starting at index 0, computing all face centers.
inline int buildTrianglesFromWorld(
    struct face* triangle,
    int B[][worldDepth][worldWidth],
    const struct block* mat,
    int matCount,
    int targetResolution
) {
    struct block currentMaterial = {};
    int resolution;
    int ti = 0;
    struct intVector coord;
    for (int z = 0; z < worldHeight; z++) {
        for (int y = 0; y < worldDepth; y++) {
            for (int x = 0; x < worldWidth; x++) {
                if (B[z][y][x] == 0) continue;

                int highestRes = 0;
                for (int i = 0; i < matCount; i++) {
                    if (mat[i].ID == B[z][y][x] &&
                        mat[i].PixelResolution > highestRes &&
                        mat[i].PixelResolution <= targetResolution) {
                        highestRes = mat[i].PixelResolution;
                    }
                }
                for (int i = 0; i < matCount; i++) {
                    if (mat[i].ID == B[z][y][x] && mat[i].PixelResolution == highestRes) {
                        currentMaterial = mat[i];
                    }
                }

                resolution = currentMaterial.PixelResolution;
                if (resolution == 0) resolution = 1;
                double d = 1.0 / (double)resolution;

                for (int row = 0; row < resolution; row++) {
                    for (int col = 0; col < resolution; col++) {
                        int ci = resolution * row + col;
                        coord = {x, y, z};


                        if(y+1<=worldDepth&&isBlockTransparent(mat, matCount, B[z][y + 1][x])){
                        // front 1      RED     Y + 1
                        triangle[ti+0].P1 = {(double)x + col*d,       (double)y+1.0, (double)z+1.0-(row+1)*d};
                        triangle[ti+0].P2 = {(double)x + (col+1)*d,   (double)y+1.0, (double)z+1.0-row*d};
                        triangle[ti+0].P3 = {(double)x + (col+1)*d,   (double)y+1.0, (double)z+1.0-(row+1)*d};
                        triangle[ti+0].Colour = currentMaterial.frontColor[ci];
                        if(doTestColors) triangle[ti+0].Colour = RED;
                        triangle[ti+0].associatedBlockCoordinates = coord;

                        // front 2
                        triangle[ti+1].P1 = {(double)x + col*d,       (double)y+1.0, (double)z+1.0-(row+1)*d};
                        triangle[ti+1].P2 = {(double)x + col*d,       (double)y+1.0, (double)z+1.0-row*d};
                        triangle[ti+1].P3 = {(double)x + (col+1)*d,   (double)y+1.0, (double)z+1.0-row*d};
                        triangle[ti+1].Colour = currentMaterial.frontColor[ci];
                        if(doTestColors) triangle[ti+1].Colour = RED;
                        triangle[ti+1].associatedBlockCoordinates = coord;
                        
                        triangle[ti+0].Center = faceCenter(triangle[ti+0]);
                        triangle[ti+1].Center = faceCenter(triangle[ti+1]);

                        ti+=2;
                        }





                        if(y-1>=0&&isBlockTransparent(mat, matCount, B[z][y - 1][x])){
                        // back 1       GREEN   Y - 1
                        triangle[ti+0].P1 = {(double)x + col*d,       (double)y, (double)z+1.0-(row+1)*d};
                        triangle[ti+0].P2 = {(double)x + (col+1)*d,   (double)y, (double)z+1.0-(row+1)*d};
                        triangle[ti+0].P3 = {(double)x + (col+1)*d,   (double)y, (double)z+1.0-row*d};
                        triangle[ti+0].Colour = currentMaterial.backColor[ci];
                        if(doTestColors) triangle[ti+2].Colour = GREEN;
                        triangle[ti+0].associatedBlockCoordinates = coord;

                        // back 2
                        triangle[ti+1].P1 = {(double)x + col*d,       (double)y, (double)z+1.0-(row+1)*d};
                        triangle[ti+1].P2 = {(double)x + (col+1)*d,   (double)y, (double)z+1.0-row*d};
                        triangle[ti+1].P3 = {(double)x + col*d,       (double)y, (double)z+1.0-row*d};
                        triangle[ti+1].Colour = currentMaterial.backColor[ci];
                        if(doTestColors) triangle[ti+3].Colour = GREEN;
                        triangle[ti+1].associatedBlockCoordinates = coord;
                        
                        triangle[ti+0].Center = faceCenter(triangle[ti+0]);
                        triangle[ti+1].Center = faceCenter(triangle[ti+1]);

                        ti+=2;
                        }    




                        if(x-1>=0&&isBlockTransparent(mat, matCount, B[z][y][x - 1])){    
                        // left 1       BLUE    X - 1
                        triangle[ti+0].P1 = {(double)x, (double)y + col*d,     (double)z+1.0-(row+1)*d};
                        triangle[ti+0].P2 = {(double)x, (double)y + (col+1)*d, (double)z+1.0-row*d};
                        triangle[ti+0].P3 = {(double)x, (double)y + (col+1)*d, (double)z+1.0-(row+1)*d};
                        triangle[ti+0].Colour = currentMaterial.leftColor[ci];
                        if(doTestColors) triangle[ti+4].Colour = BLUE;
                        triangle[ti+0].associatedBlockCoordinates = coord;

                        // left 2
                        triangle[ti+1].P1 = {(double)x, (double)y + col*d,     (double)z+1.0-(row+1)*d};
                        triangle[ti+1].P2 = {(double)x, (double)y + col*d,     (double)z+1.0-row*d};
                        triangle[ti+1].P3 = {(double)x, (double)y + (col+1)*d, (double)z+1.0-row*d};
                        triangle[ti+1].Colour = currentMaterial.leftColor[ci];
                        if(doTestColors) triangle[ti+5].Colour = BLUE;
                        triangle[ti+1].associatedBlockCoordinates = coord;
                        
                        triangle[ti+0].Center = faceCenter(triangle[ti+0]);
                        triangle[ti+1].Center = faceCenter(triangle[ti+1]);

                        ti+=2;
                        }  
                        
                        


                        if(x+1<=worldWidth&&isBlockTransparent(mat, matCount, B[z][y][x + 1])){    
                        // right 1      BLACK   X + 1
                        triangle[ti+0].P1 = {(double)x+1.0, (double)y + col*d,     (double)z+1.0-(row+1)*d};
                        triangle[ti+0].P2 = {(double)x+1.0, (double)y + (col+1)*d, (double)z+1.0-(row+1)*d};
                        triangle[ti+0].P3 = {(double)x+1.0, (double)y + (col+1)*d, (double)z+1.0-row*d};
                        triangle[ti+0].Colour = currentMaterial.rightColor[ci];
                        if(doTestColors) triangle[ti+6].Colour = BLACK;
                        triangle[ti+0].associatedBlockCoordinates = coord;

                        // right 2
                        triangle[ti+1].P1 = {(double)x+1.0, (double)y + col*d,     (double)z+1.0-(row+1)*d};
                        triangle[ti+1].P2 = {(double)x+1.0, (double)y + (col+1)*d, (double)z+1.0-row*d};
                        triangle[ti+1].P3 = {(double)x+1.0, (double)y + col*d,     (double)z+1.0-row*d};
                        triangle[ti+1].Colour = currentMaterial.rightColor[ci];
                        if(doTestColors) triangle[ti+7].Colour = BLACK;
                        triangle[ti+1].associatedBlockCoordinates = coord;
                        
                        triangle[ti+0].Center = faceCenter(triangle[ti+0]);
                        triangle[ti+1].Center = faceCenter(triangle[ti+1]);

                        ti+=2;
                        }
                        
                        


                        if(z+1<=worldHeight&&isBlockTransparent(mat, matCount, B[z + 1][y][x])){    
                        // top 1        WHITE   Z + 1
                        triangle[ti+0].P1 = {(double)x+1.0-(row+1)*d, (double)y + col*d,     (double)z+1.0};
                        triangle[ti+0].P2 = {(double)x+1.0-row*d,     (double)y + (col+1)*d, (double)z+1.0};
                        triangle[ti+0].P3 = {(double)x+1.0-(row+1)*d, (double)y + (col+1)*d, (double)z+1.0};
                        triangle[ti+0].Colour = currentMaterial.topColor[ci];
                        if(doTestColors) triangle[ti+8].Colour = WHITE;
                        triangle[ti+0].associatedBlockCoordinates = coord;

                        // top 2
                        triangle[ti+1].P1 = {(double)x+1.0-(row+1)*d, (double)y + col*d,     (double)z+1.0};
                        triangle[ti+1].P2 = {(double)x+1.0-row*d,     (double)y + col*d,     (double)z+1.0};
                        triangle[ti+1].P3 = {(double)x+1.0-row*d,     (double)y + (col+1)*d, (double)z+1.0};
                        triangle[ti+1].Colour = currentMaterial.topColor[ci];
                        if(doTestColors) triangle[ti+9].Colour = WHITE;
                        triangle[ti+1].associatedBlockCoordinates = coord;
                        
                        triangle[ti+0].Center = faceCenter(triangle[ti+0]);
                        triangle[ti+1].Center = faceCenter(triangle[ti+1]);

                        ti+=2;
                        }
                        
                        


                        if(z-1>=0&&isBlockTransparent(mat, matCount, B[z - 1][y][x])){    
                        // bottom 1     GRAY    Z - 1
                        triangle[ti+0].P1 = {(double)x+1.0-(row+1)*d, (double)y + col*d,     (double)z};
                        triangle[ti+0].P2 = {(double)x+1.0-(row+1)*d, (double)y + (col+1)*d, (double)z};
                        triangle[ti+0].P3 = {(double)x+1.0-row*d,     (double)y + (col+1)*d, (double)z};
                        triangle[ti+0].Colour = currentMaterial.bottomColor[ci];
                        if(doTestColors) triangle[ti+10].Colour = GRAY;
                        triangle[ti+0].associatedBlockCoordinates = coord;

                        // bottom 2
                        triangle[ti+1].P1 = {(double)x+1.0-(row+1)*d, (double)y + col*d,     (double)z};
                        triangle[ti+1].P2 = {(double)x+1.0-row*d,     (double)y + (col+1)*d, (double)z};
                        triangle[ti+1].P3 = {(double)x+1.0-row*d,     (double)y + col*d,     (double)z};
                        triangle[ti+1].Colour = currentMaterial.bottomColor[ci];
                        if(doTestColors) triangle[ti+11].Colour = GRAY;
                        triangle[ti+1].associatedBlockCoordinates = coord;
                        
                        triangle[ti+0].Center = faceCenter(triangle[ti+0]);
                        triangle[ti+1].Center = faceCenter(triangle[ti+1]);

                        ti+=2;
                        }                        
                    }
                }
            }
        }
    }

    return ti;
}

// Builds triangles for a single block at (x, y, z) with the given material,
// appending them to triangle[] starting at index startIndex.
// Returns the number of triangles written.
inline int buildTrianglesForBlock(
    struct face* triangle,
    int B[][worldDepth][worldWidth],
    int startIndex,
    int x, int y, int z,
    const struct block& matToPlaceHere,
    const struct block* mat,
    int matCount,
    int targetResolution
) {
    int resolution = matToPlaceHere.PixelResolution;
    if(resolution == 0) resolution = 1;
    double d = 1.0 / (double)resolution;
    int ti = startIndex;
    struct intVector coord = {x, y, z};

    if(x<0||x>worldWidth||y<0||y>worldDepth||z<0||z>worldHeight){return 0;}

    for(int row = 0; row < resolution; row++) {
        for(int col = 0; col < resolution; col++){
            int ci = resolution * row + col;
            coord = {x, y, z};


            if(y+1<=worldDepth&&isBlockTransparent(mat, matCount, B[z][y + 1][x])){
                // front 1      RED     Y + 1
                triangle[ti+0].P1 = {(double)x + col*d,       (double)y+1.0, (double)z+1.0-(row+1)*d};
                triangle[ti+0].P2 = {(double)x + (col+1)*d,   (double)y+1.0, (double)z+1.0-row*d};
                triangle[ti+0].P3 = {(double)x + (col+1)*d,   (double)y+1.0, (double)z+1.0-(row+1)*d};
                triangle[ti+0].Colour = matToPlaceHere.frontColor[ci];
                if(doTestColors) triangle[ti+0].Colour = RED;
                triangle[ti+0].associatedBlockCoordinates = coord;

                // front 2
                triangle[ti+1].P1 = {(double)x + col*d,       (double)y+1.0, (double)z+1.0-(row+1)*d};
                triangle[ti+1].P2 = {(double)x + col*d,       (double)y+1.0, (double)z+1.0-row*d};
                triangle[ti+1].P3 = {(double)x + (col+1)*d,   (double)y+1.0, (double)z+1.0-row*d};
                triangle[ti+1].Colour = matToPlaceHere.frontColor[ci];
                if(doTestColors) triangle[ti+1].Colour = RED;
                triangle[ti+1].associatedBlockCoordinates = coord;
                        
                triangle[ti+0].Center = faceCenter(triangle[ti+0]);
                triangle[ti+1].Center = faceCenter(triangle[ti+1]);

                ti+=2;
            }





            if(y-1>=0&&isBlockTransparent(mat, matCount, B[z][y - 1][x])){
                // back 1       GREEN   Y - 1
                triangle[ti+0].P1 = {(double)x + col*d,       (double)y, (double)z+1.0-(row+1)*d};
                triangle[ti+0].P2 = {(double)x + (col+1)*d,   (double)y, (double)z+1.0-(row+1)*d};
                triangle[ti+0].P3 = {(double)x + (col+1)*d,   (double)y, (double)z+1.0-row*d};
                triangle[ti+0].Colour = matToPlaceHere.backColor[ci];
                if(doTestColors) triangle[ti+2].Colour = GREEN;
                triangle[ti+0].associatedBlockCoordinates = coord;

                // back 2
                triangle[ti+1].P1 = {(double)x + col*d,       (double)y, (double)z+1.0-(row+1)*d};
                triangle[ti+1].P2 = {(double)x + (col+1)*d,   (double)y, (double)z+1.0-row*d};
                triangle[ti+1].P3 = {(double)x + col*d,       (double)y, (double)z+1.0-row*d};
                triangle[ti+1].Colour = matToPlaceHere.backColor[ci];
                if(doTestColors) triangle[ti+3].Colour = GREEN;
                triangle[ti+1].associatedBlockCoordinates = coord;
                        
                triangle[ti+0].Center = faceCenter(triangle[ti+0]);
                triangle[ti+1].Center = faceCenter(triangle[ti+1]);

                ti+=2;
            }    




            if(x-1>=0&&isBlockTransparent(mat, matCount, B[z][y][x-1])){    
                // left 1       BLUE    X - 1
                triangle[ti+0].P1 = {(double)x, (double)y + col*d,     (double)z+1.0-(row+1)*d};
                triangle[ti+0].P2 = {(double)x, (double)y + (col+1)*d, (double)z+1.0-row*d};
                triangle[ti+0].P3 = {(double)x, (double)y + (col+1)*d, (double)z+1.0-(row+1)*d};
                triangle[ti+0].Colour = matToPlaceHere.leftColor[ci];
                if(doTestColors) triangle[ti+4].Colour = BLUE;
                triangle[ti+0].associatedBlockCoordinates = coord;

                // left 2
                triangle[ti+1].P1 = {(double)x, (double)y + col*d,     (double)z+1.0-(row+1)*d};
                triangle[ti+1].P2 = {(double)x, (double)y + col*d,     (double)z+1.0-row*d};
                triangle[ti+1].P3 = {(double)x, (double)y + (col+1)*d, (double)z+1.0-row*d};
                triangle[ti+1].Colour = matToPlaceHere.leftColor[ci];
                if(doTestColors) triangle[ti+5].Colour = BLUE;
                triangle[ti+1].associatedBlockCoordinates = coord;
                        
                triangle[ti+0].Center = faceCenter(triangle[ti+0]);
                triangle[ti+1].Center = faceCenter(triangle[ti+1]);

                ti+=2;
            }  
                        
                        


            if(x+1<=worldWidth&&isBlockTransparent(mat, matCount, B[z][y][x + 1])){    
                // right 1      BLACK   X + 1
                triangle[ti+0].P1 = {(double)x+1.0, (double)y + col*d,     (double)z+1.0-(row+1)*d};
                triangle[ti+0].P2 = {(double)x+1.0, (double)y + (col+1)*d, (double)z+1.0-(row+1)*d};
                triangle[ti+0].P3 = {(double)x+1.0, (double)y + (col+1)*d, (double)z+1.0-row*d};
                triangle[ti+0].Colour = matToPlaceHere.rightColor[ci];
                if(doTestColors) triangle[ti+6].Colour = BLACK;
                triangle[ti+0].associatedBlockCoordinates = coord;

                // right 2
                triangle[ti+1].P1 = {(double)x+1.0, (double)y + col*d,     (double)z+1.0-(row+1)*d};
                triangle[ti+1].P2 = {(double)x+1.0, (double)y + (col+1)*d, (double)z+1.0-row*d};
                triangle[ti+1].P3 = {(double)x+1.0, (double)y + col*d,     (double)z+1.0-row*d};
                triangle[ti+1].Colour = matToPlaceHere.rightColor[ci];
                if(doTestColors) triangle[ti+7].Colour = BLACK;
                triangle[ti+1].associatedBlockCoordinates = coord;
                        
                triangle[ti+0].Center = faceCenter(triangle[ti+0]);
                triangle[ti+1].Center = faceCenter(triangle[ti+1]);

                ti+=2;
            }
                        
                        


            if(z+1<=worldHeight&&isBlockTransparent(mat, matCount, B[z + 1][y][x])){    
                // top 1        WHITE   Z + 1
                triangle[ti+0].P1 = {(double)x+1.0-(row+1)*d, (double)y + col*d,     (double)z+1.0};
                triangle[ti+0].P2 = {(double)x+1.0-row*d,     (double)y + (col+1)*d, (double)z+1.0};
                triangle[ti+0].P3 = {(double)x+1.0-(row+1)*d, (double)y + (col+1)*d, (double)z+1.0};
                triangle[ti+0].Colour = matToPlaceHere.topColor[ci];
                if(doTestColors) triangle[ti+8].Colour = WHITE;
                triangle[ti+0].associatedBlockCoordinates = coord;

                // top 2
                triangle[ti+1].P1 = {(double)x+1.0-(row+1)*d, (double)y + col*d,     (double)z+1.0};
                triangle[ti+1].P2 = {(double)x+1.0-row*d,     (double)y + col*d,     (double)z+1.0};
                triangle[ti+1].P3 = {(double)x+1.0-row*d,     (double)y + (col+1)*d, (double)z+1.0};
                triangle[ti+1].Colour = matToPlaceHere.topColor[ci];
                if(doTestColors) triangle[ti+9].Colour = WHITE;
                triangle[ti+1].associatedBlockCoordinates = coord;
                        
                triangle[ti+0].Center = faceCenter(triangle[ti+0]);
                triangle[ti+1].Center = faceCenter(triangle[ti+1]);

                ti+=2;
            }
                        
                        


            if(z-1>=0&&isBlockTransparent(mat, matCount, B[z - 1][y][x])){    
                // bottom 1     GRAY    Z - 1
                triangle[ti+0].P1 = {(double)x+1.0-(row+1)*d, (double)y + col*d,     (double)z};
                triangle[ti+0].P2 = {(double)x+1.0-(row+1)*d, (double)y + (col+1)*d, (double)z};
                triangle[ti+0].P3 = {(double)x+1.0-row*d,     (double)y + (col+1)*d, (double)z};
                triangle[ti+0].Colour = matToPlaceHere.bottomColor[ci];
                if(doTestColors) triangle[ti+10].Colour = GRAY;
                triangle[ti+0].associatedBlockCoordinates = coord;

                // bottom 2
                triangle[ti+1].P1 = {(double)x+1.0-(row+1)*d, (double)y + col*d,     (double)z};
                triangle[ti+1].P2 = {(double)x+1.0-row*d,     (double)y + (col+1)*d, (double)z};
                triangle[ti+1].P3 = {(double)x+1.0-row*d,     (double)y + col*d,     (double)z};
                triangle[ti+1].Colour = matToPlaceHere.bottomColor[ci];
                if(doTestColors) triangle[ti+11].Colour = GRAY;
                triangle[ti+1].associatedBlockCoordinates = coord;
                        
                triangle[ti+0].Center = faceCenter(triangle[ti+0]);
                triangle[ti+1].Center = faceCenter(triangle[ti+1]);

                ti+=2;
            }                        
        }
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        /*{
            int ci = resolution * row + col;

            triangle[ti+0].P1 = {(double)x + col*d,       (double)y+1.0, (double)z+1.0-(row+1)*d};
            triangle[ti+0].P2 = {(double)x + (col+1)*d,   (double)y+1.0, (double)z+1.0-row*d};
            triangle[ti+0].P3 = {(double)x + (col+1)*d,   (double)y+1.0, (double)z+1.0-(row+1)*d};
            triangle[ti+0].Colour = mat.frontColor[ci];

            triangle[ti+1].P1 = {(double)x + col*d,       (double)y+1.0, (double)z+1.0-(row+1)*d};
            triangle[ti+1].P2 = {(double)x + col*d,       (double)y+1.0, (double)z+1.0-row*d};
            triangle[ti+1].P3 = {(double)x + (col+1)*d,   (double)y+1.0, (double)z+1.0-row*d};
            triangle[ti+1].Colour = mat.frontColor[ci];

            triangle[ti+2].P1 = {(double)x + col*d,       (double)y, (double)z+1.0-(row+1)*d};
            triangle[ti+2].P2 = {(double)x + (col+1)*d,   (double)y, (double)z+1.0-(row+1)*d};
            triangle[ti+2].P3 = {(double)x + (col+1)*d,   (double)y, (double)z+1.0-row*d};
            triangle[ti+2].Colour = mat.backColor[ci];

            triangle[ti+3].P1 = {(double)x + col*d,       (double)y, (double)z+1.0-(row+1)*d};
            triangle[ti+3].P2 = {(double)x + (col+1)*d,   (double)y, (double)z+1.0-row*d};
            triangle[ti+3].P3 = {(double)x + col*d,       (double)y, (double)z+1.0-row*d};
            triangle[ti+3].Colour = mat.backColor[ci];

            triangle[ti+4].P1 = {(double)x, (double)y + col*d,     (double)z+1.0-(row+1)*d};
            triangle[ti+4].P2 = {(double)x, (double)y + (col+1)*d, (double)z+1.0-row*d};
            triangle[ti+4].P3 = {(double)x, (double)y + (col+1)*d, (double)z+1.0-(row+1)*d};
            triangle[ti+4].Colour = mat.leftColor[ci];

            triangle[ti+5].P1 = {(double)x, (double)y + col*d,     (double)z+1.0-(row+1)*d};
            triangle[ti+5].P2 = {(double)x, (double)y + col*d,     (double)z+1.0-row*d};
            triangle[ti+5].P3 = {(double)x, (double)y + (col+1)*d, (double)z+1.0-row*d};
            triangle[ti+5].Colour = mat.leftColor[ci];

            triangle[ti+6].P1 = {(double)x+1.0, (double)y + col*d,     (double)z+1.0-(row+1)*d};
            triangle[ti+6].P2 = {(double)x+1.0, (double)y + (col+1)*d, (double)z+1.0-(row+1)*d};
            triangle[ti+6].P3 = {(double)x+1.0, (double)y + (col+1)*d, (double)z+1.0-row*d};
            triangle[ti+6].Colour = mat.rightColor[ci];

            triangle[ti+7].P1 = {(double)x+1.0, (double)y + col*d,     (double)z+1.0-(row+1)*d};
            triangle[ti+7].P2 = {(double)x+1.0, (double)y + (col+1)*d, (double)z+1.0-row*d};
            triangle[ti+7].P3 = {(double)x+1.0, (double)y + col*d,     (double)z+1.0-row*d};
            triangle[ti+7].Colour = mat.rightColor[ci];

            triangle[ti+8].P1  = {(double)x+1.0-(row+1)*d, (double)y + col*d,     (double)z+1.0};
            triangle[ti+8].P2  = {(double)x+1.0-row*d,     (double)y + (col+1)*d, (double)z+1.0};
            triangle[ti+8].P3  = {(double)x+1.0-(row+1)*d, (double)y + (col+1)*d, (double)z+1.0};
            triangle[ti+8].Colour = mat.topColor[ci];

            triangle[ti+9].P1  = {(double)x+1.0-(row+1)*d, (double)y + col*d,     (double)z+1.0};
            triangle[ti+9].P2  = {(double)x+1.0-row*d,     (double)y + col*d,     (double)z+1.0};
            triangle[ti+9].P3  = {(double)x+1.0-row*d,     (double)y + (col+1)*d, (double)z+1.0};
            triangle[ti+9].Colour = mat.topColor[ci];

            triangle[ti+10].P1 = {(double)x+1.0-(row+1)*d, (double)y + col*d,     (double)z};
            triangle[ti+10].P2 = {(double)x+1.0-(row+1)*d, (double)y + (col+1)*d, (double)z};
            triangle[ti+10].P3 = {(double)x+1.0-row*d,     (double)y + (col+1)*d, (double)z};
            triangle[ti+10].Colour = mat.bottomColor[ci];

            triangle[ti+11].P1 = {(double)x+1.0-(row+1)*d, (double)y + col*d,     (double)z};
            triangle[ti+11].P2 = {(double)x+1.0-row*d,     (double)y + (col+1)*d, (double)z};
            triangle[ti+11].P3 = {(double)x+1.0-row*d,     (double)y + col*d,     (double)z};
            triangle[ti+11].Colour = mat.bottomColor[ci];

            for(int k = 0; k < 12; k++) {
                triangle[ti+k].Center = faceCenter(triangle[ti+k]);
                triangle[ti+k].associatedBlockCoordinates = coord;
            }

            ti += 12;
        }*/
    }

    return ti - startIndex;
}

#endif // BUILD_WORLD_H
