#include <list>

class UGLGraph
{
    //typedef huys::ADT::UVector<float> UDataVector;
    typedef std::list<float> UDataList;
public:
    //
    static UGLGraph * instance()
    {

        if (_instance == 0)
        {
            static UGLGraph uglg;
            _instance = &uglg;
        }
        return _instance;
    }
    //
    void setup( int width, int height, int offsetX, int offsetY,
                int scaleX, int scaleY, int channels, int cacheSize )
    {
        _offsetX = offsetX;
        _offsetY = offsetY;
        _width = width;
        _height = height;
        _scaleX = scaleX;
        _scaleY = scaleY;
        _channels = channels;    // ToDo
        _cacheSize = cacheSize;
    }

    //
    void update( float data);
    void draw();
private:
    static UGLGraph * _instance;
    //
    UGLGraph() {}
    //
    int _cacheSize;
    int _pWidth, _pHeight;    // in percentages of existing window sizes
    int _width, _height;
    int _channels;
    int _scaleX, _scaleY;
    int _offsetX, _offsetY;
    UDataList _data;
    UDataList _movingAverage;
    UDataList::const_iterator _iterData;
    UDataList::const_iterator _iterMA;
};
