#include "opencv/cv.h"
#include "opencv/highgui.h"
using std::vector;
using cv::Scalar;

struct grid
{
    unsigned int width, height, channels;
    vector<vector<vector<unsigned char> > > data;
    grid(unsigned int width, unsigned int height, unsigned int channels);
    ~grid();
};

struct blob
{
    vector<vector<bool> > point_array;
    unsigned int bbox_l, bbox_t, bbox_r, bbox_b;
    unsigned int x, y, area, cx, cy, id;
    bool pressed, released;
    blob(unsigned int width, unsigned int height);
    Scalar color;
    //~blob();
};

struct node
{
    unsigned int label, rank;
    node* parent;
    node(unsigned int label);
    //~node();
};

struct pixel
{
    unsigned int x, y;
    node* label;
    pixel(unsigned int x, unsigned int y, node* label);
    //~pixel();
};

void MakeSet(node* x);
node* UFind(node* x);
void Union(node* x,node* y);
void calculateCentroid( blob* blob );
