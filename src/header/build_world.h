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


                        if(y+1<worldDepth&&isBlockTransparent(mat, matCount, B[z][y + 1][x])){
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
                        
                        


                        if(x+1<worldWidth&&isBlockTransparent(mat, matCount, B[z][y][x + 1])){    
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
                        
                        


                        if(z+1<worldHeight&&isBlockTransparent(mat, matCount, B[z + 1][y][x])){    
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

    if(x<0||x>=worldWidth||y<0||y>=worldDepth||z<0||z>=worldHeight){return 0;}

    for(int row = 0; row < resolution; row++) {
        for(int col = 0; col < resolution; col++){
            int ci = resolution * row + col;
            coord = {x, y, z};


            if(y+1<worldDepth&&isBlockTransparent(mat, matCount, B[z][y + 1][x])){
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
                        
                        


            if(x+1<worldWidth&&isBlockTransparent(mat, matCount, B[z][y][x + 1])){    
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
                        
                        


            if(z+1<worldHeight&&isBlockTransparent(mat, matCount, B[z + 1][y][x])){    
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

// Partitions triangle[0, count) so opaque faces (alpha == 255) come first,
// followed by translucent faces. One-time O(count) operation used after a
// full rebuild (initial load, F2 world switch). Returns the opaque count;
// the translucent count is (count - returned value).
inline int partitionTriangles(struct face* triangle, int count) {
    int lo = 0, hi = count;
    while(lo < hi) {
        if(triangle[lo].Colour.a == 255) {
            lo++;
        } else {
            hi--;
            struct face tmp = triangle[lo];
            triangle[lo] = triangle[hi];
            triangle[hi] = tmp;
        }
    }
    return lo;
}

// Inserts face f into triangle[], keeping triangle[0, opaqueCount) opaque and
// triangle[opaqueCount, opaqueCount+translucentCount) translucent. O(1).
inline void insertFace(struct face* triangle, int& opaqueCount, int& translucentCount, const struct face& f) {
    if(f.Colour.a == 255) {
        triangle[opaqueCount + translucentCount] = triangle[opaqueCount];
        triangle[opaqueCount] = f;
        opaqueCount++;
    } else {
        triangle[opaqueCount + translucentCount] = f;
        translucentCount++;
    }
}

// Removes the face at index i, swapping in the last face of its region
// (opaque or translucent) to keep both regions contiguous. O(1).
inline void removeFaceAt(struct face* triangle, int& opaqueCount, int& translucentCount, int i) {
    if(i < opaqueCount) {
        triangle[i] = triangle[opaqueCount - 1];
        if(translucentCount > 0)
            triangle[opaqueCount - 1] = triangle[opaqueCount + translucentCount - 1];
        opaqueCount--;
    } else {
        triangle[i] = triangle[opaqueCount + translucentCount - 1];
        translucentCount--;
    }
}

// Removes every face whose associatedBlockCoordinates matches one of coords[0..coordCount).
inline void removeFacesForBlocks(struct face* triangle, int& opaqueCount, int& translucentCount,
                                  const struct intVector* coords, int coordCount) {
    int i = 0;
    while(i < opaqueCount + translucentCount) {
        struct intVector c = triangle[i].associatedBlockCoordinates;
        bool match = false;
        for(int k = 0; k < coordCount; k++) {
            if(c.x == coords[k].x && c.y == coords[k].y && c.z == coords[k].z) { match = true; break; }
        }
        if(match) removeFaceAt(triangle, opaqueCount, translucentCount, i);
        else i++;
    }
}

// Builds the faces for block (x,y,z) and inserts each into triangle[] via insertFace.
inline void placeBlockFaces(struct face* triangle, int& opaqueCount, int& translucentCount,
                             int B[][worldDepth][worldWidth],
                             int x, int y, int z, const struct block& matToPlaceHere,
                             const struct block* mat, int matCount) {
    struct face scratch[12 * targetResolution * targetResolution];
    int n = buildTrianglesForBlock(scratch, B, 0, x, y, z, matToPlaceHere, mat, matCount, targetResolution);
    for(int i = 0; i < n; i++) insertFace(triangle, opaqueCount, translucentCount, scratch[i]);
}

// Sets B[z][y][x] = blockID and rebuilds the faces of that block plus all
// in-bounds neighbors, so faces hidden or exposed by the change stay correct.
// The single entry point for block breaking/placing and the setblock/fill commands.
inline void setBlock(struct face* triangle, int& opaqueCount, int& translucentCount,
                      int B[][worldDepth][worldWidth], int x, int y, int z, int blockID,
                      const struct block* mat, int matCount, int& blockCount) {
    if(B[z][y][x] == blockID) return;

    struct intVector coords[7];
    int coordCount = 0;
    coords[coordCount++] = {x, y, z};
    static const int dx[] = {1,-1,0,0,0,0}, dy[] = {0,0,1,-1,0,0}, dz[] = {0,0,0,0,1,-1};
    for(int n = 0; n < 6; n++) {
        int nx = x+dx[n], ny = y+dy[n], nz = z+dz[n];
        if(nx>=0 && nx<worldWidth && ny>=0 && ny<worldDepth && nz>=0 && nz<worldHeight)
            coords[coordCount++] = {nx, ny, nz};
    }

    if(B[z][y][x] != 0) blockCount--;
    if(blockID     != 0) blockCount++;

    removeFacesForBlocks(triangle, opaqueCount, translucentCount, coords, coordCount);
    B[z][y][x] = blockID;

    for(int k = 0; k < coordCount; k++) {
        int bx = coords[k].x, by = coords[k].y, bz = coords[k].z;
        int id = B[bz][by][bx];
        if(id != 0) placeBlockFaces(triangle, opaqueCount, translucentCount, B, bx, by, bz, getMaterial(mat, matCount, id), mat, matCount);
    }
}

#endif // BUILD_WORLD_H
