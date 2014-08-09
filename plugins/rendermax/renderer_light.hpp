#ifndef RENDERER_LIGHT_INCLUDED
#define RENDERER_LIGHT_INCLUDED
#include "renderer_opengl.hpp"
#include "Types.h"
<<<<<<< HEAD
#include <tuple>
#include <stack>
#include <memory>
#include <unordered_map>
// we are not using boost so let's cheat:
template <class T>
inline void hash_combine(std::size_t & seed, const T & v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std
{
    template<typename S, typename T> struct hash<pair<S, T>>
    {
        inline size_t operator()(const pair<S, T> & v) const
        {
            size_t seed = 0;
            ::hash_combine(seed, v.first);
            ::hash_combine(seed, v.second);
            return seed;
        }
    };
    template<typename S, typename T,typename V> struct hash<tuple<S, T, V>>
    {
        inline size_t operator()(const tuple<S, T, V> & v) const
        {
            size_t seed = 0;
            ::hash_combine(seed,get<0>(v));
            ::hash_combine(seed,get<1>(v));
            ::hash_combine(seed,get<2>(v));
            return seed;
        }
    };
}
// now we can hash pairs and tuples

#include "modules/MapCache.h"
bool isInRect(const df::coord2d& pos,const DFHack::rect2d& rect);
struct renderer_light : public renderer_wrap {
private:
    float light_adaptation;
    rgbf adapt_to_light(const rgbf& light)
    {
        const float influence=0.0001f;
        const float max_adapt=1;
        const float min_adapt=0;
        float intensity=(light.r+light.g+light.b)/3.0;
        light_adaptation=intensity*influence+light_adaptation*(1-influence);
        float delta=light_adaptation-intensity;
        
        rgbf ret;
        ret.r=light.r-delta;
        ret.g=light.g-delta;
        ret.b=light.b-delta;
        return ret;
        //if light_adaptation/intensity~1 then draw 1,1,1 (i.e. totally adapted)
        /*
            1. adapted -> 1,1,1 (full bright everything okay) delta=0 multiplier=?
            2. light adapted, real=dark -> darker delta>0   multiplier<1
            3. dark adapted, real=light -> lighter delta<0  multiplier>1
        */
        //if light_adaptation/intensity!=0 then draw 

    }
=======
#include <map>
#include <tuple>
#include "modules/MapCache.h"

struct renderer_light : public renderer_wrap {
private:
>>>>>>> 2779290b70feebb0fab5bd7225a18604efaf5cc9
    void colorizeTile(int x,int y)
    {
        const int tile = x*(df::global::gps->dimy) + y;
        old_opengl* p=reinterpret_cast<old_opengl*>(parent);
        float *fg = p->fg + tile * 4 * 6;
        float *bg = p->bg + tile * 4 * 6;
        float *tex = p->tex + tile * 2 * 6;
<<<<<<< HEAD
        rgbf light=lightGrid[tile];//for light adaptation: rgbf light=adapt_to_light(lightGrid[tile]);

        for (int i = 0; i < 6; i++) { //oh how sse would do wonders here, or shaders...
=======
        lightCell light=lightGrid[tile];
        for (int i = 0; i < 6; i++) {
>>>>>>> 2779290b70feebb0fab5bd7225a18604efaf5cc9
            *(fg++) *= light.r;
            *(fg++) *= light.g;
            *(fg++) *= light.b;
            *(fg++) = 1;

            *(bg++) *= light.r;
            *(bg++) *= light.g;
            *(bg++) *= light.b;
            *(bg++) = 1;
        }
    }
    void reinitLightGrid(int w,int h)
    {
        tthread::lock_guard<tthread::fast_mutex> guard(dataMutex);
<<<<<<< HEAD
        lightGrid.resize(w*h,rgbf(1,1,1));
=======
        lightGrid.resize(w*h);
>>>>>>> 2779290b70feebb0fab5bd7225a18604efaf5cc9
    }
    void reinitLightGrid()
    {
        reinitLightGrid(df::global::gps->dimy,df::global::gps->dimx);
    }
<<<<<<< HEAD
    
public:
    tthread::fast_mutex dataMutex;
    std::vector<rgbf> lightGrid;
    renderer_light(renderer* parent):renderer_wrap(parent),light_adaptation(1)
=======
public:
    tthread::fast_mutex dataMutex;
    std::vector<lightCell> lightGrid;
    renderer_light(renderer* parent):renderer_wrap(parent)
>>>>>>> 2779290b70feebb0fab5bd7225a18604efaf5cc9
    {
        reinitLightGrid();
    }
    virtual void update_tile(int32_t x, int32_t y) { 
        renderer_wrap::update_tile(x,y);
        tthread::lock_guard<tthread::fast_mutex> guard(dataMutex);
        colorizeTile(x,y);
    };
    virtual void update_all() { 
        renderer_wrap::update_all();
        tthread::lock_guard<tthread::fast_mutex> guard(dataMutex);
        for (int x = 0; x < df::global::gps->dimx; x++)
            for (int y = 0; y < df::global::gps->dimy; y++)
                colorizeTile(x,y);
    };
    virtual void grid_resize(int32_t w, int32_t h) { 
        renderer_wrap::grid_resize(w,h);
        reinitLightGrid(w,h);
    };
    virtual void resize(int32_t w, int32_t h) {
        renderer_wrap::resize(w,h);
        reinitLightGrid();
    }
<<<<<<< HEAD
    virtual void set_fullscreen()
    {
        renderer_wrap::set_fullscreen();
        reinitLightGrid();
    }
    virtual void zoom(df::zoom_commands z)
    {
        renderer_wrap::zoom(z);
        reinitLightGrid();
    }
=======
>>>>>>> 2779290b70feebb0fab5bd7225a18604efaf5cc9
};
class lightingEngine
{
public:
    lightingEngine(renderer_light* target):myRenderer(target){}
<<<<<<< HEAD
    virtual ~lightingEngine(){}
=======

>>>>>>> 2779290b70feebb0fab5bd7225a18604efaf5cc9
    virtual void reinit()=0;
    virtual void calculate()=0;

    virtual void updateWindow()=0;
<<<<<<< HEAD
    virtual void preRender()=0;
=======
>>>>>>> 2779290b70feebb0fab5bd7225a18604efaf5cc9

    virtual void loadSettings()=0;
    virtual void clear()=0;
    
    virtual void setHour(float h)=0;
    virtual void debug(bool enable)=0;
protected:
    renderer_light* myRenderer;
};
struct lightSource
{
<<<<<<< HEAD
    rgbf power;
=======
    lightCell power;
>>>>>>> 2779290b70feebb0fab5bd7225a18604efaf5cc9
    int radius;
    bool flicker;
    lightSource():power(0,0,0),radius(0),flicker(false)
    {

    }
<<<<<<< HEAD
    lightSource(rgbf power,int radius);
=======
    lightSource(lightCell power,int radius);
>>>>>>> 2779290b70feebb0fab5bd7225a18604efaf5cc9
    float powerSquared()const
    {
        return power.r*power.r+power.g*power.g+power.b*power.b;
    }
    void combine(const lightSource& other);

};
struct matLightDef
{
    bool isTransparent;
<<<<<<< HEAD
    rgbf transparency;
    bool isEmiting;
    bool flicker;
    rgbf emitColor;
    int radius;
    matLightDef():isTransparent(false),isEmiting(false),transparency(0,0,0),emitColor(0,0,0),radius(0){}
    matLightDef(rgbf transparency,rgbf emit,int rad):isTransparent(true),isEmiting(true),
        transparency(transparency),emitColor(emit),radius(rad){}
    matLightDef(rgbf emit,int rad):isTransparent(false),isEmiting(true),emitColor(emit),radius(rad),transparency(0,0,0){}
    matLightDef(rgbf transparency):isTransparent(true),isEmiting(false),transparency(transparency){}
=======
    lightCell transparency;
    bool isEmiting;
    bool flicker;
    lightCell emitColor;
    int radius;
    matLightDef():isTransparent(false),isEmiting(false),transparency(0,0,0),emitColor(0,0,0),radius(0){}
    matLightDef(lightCell transparency,lightCell emit,int rad):isTransparent(true),isEmiting(true),
        transparency(transparency),emitColor(emit),radius(rad){}
    matLightDef(lightCell emit,int rad):isTransparent(false),isEmiting(true),emitColor(emit),radius(rad),transparency(0,0,0){}
    matLightDef(lightCell transparency):isTransparent(true),isEmiting(false),transparency(transparency){}
>>>>>>> 2779290b70feebb0fab5bd7225a18604efaf5cc9
    lightSource makeSource(float size=1) const
    {
        if(size>0.999 && size<1.001)
            return lightSource(emitColor,radius);
        else
            return lightSource(emitColor*size,radius*size);//todo check if this is sane
    }
};
struct buildingLightDef
{
    matLightDef light;
    bool poweredOnly;
    bool useMaterial;
    float thickness;
    float size;
    buildingLightDef():poweredOnly(false),useMaterial(true),thickness(1.0f),size(1.0f){}
};
<<<<<<< HEAD
struct itemLightDef
{
    matLightDef light;   
    bool haul;
    bool equiped;
    bool onGround;
    bool inBuilding;
    bool inContainer;
    bool useMaterial;
    itemLightDef():haul(true),equiped(true),onGround(true),inBuilding(false),inContainer(false),useMaterial(true){}
};
struct creatureLightDef
{
    matLightDef light;

};
class lightThread;
class lightingEngineViewscreen;
class lightThreadDispatch
{
    lightingEngineViewscreen *parent;
public:
    DFHack::rect2d viewPort;

    std::vector<std::unique_ptr<lightThread> > threadPool;
    std::vector<lightSource>& lights;

    tthread::mutex occlusionMutex;
    tthread::condition_variable occlusionDone; //all threads wait for occlusion to finish
    bool occlusionReady;
    tthread::mutex unprocessedMutex;
    std::stack<DFHack::rect2d> unprocessed; //stack of parts of map where lighting is not finished
    std::vector<rgbf>& occlusion;
    int& num_diffusion;

    tthread::mutex writeLock; //mutex for lightMap
    std::vector<rgbf>& lightMap;

    tthread::condition_variable writesDone;
    int writeCount;

    lightThreadDispatch(lightingEngineViewscreen* p);
    ~lightThreadDispatch();
    void signalDoneOcclusion();
    void shutdown();
    void waitForWrites();

    int getW();
    int getH();
    void start(int count);
};
class lightThread
{
    std::vector<rgbf> canvas;
    lightThreadDispatch& dispatch;
    DFHack::rect2d myRect;
    void work(); //main light calculation function
    void combine(); //combine existing canvas into global lightmap
public:
    tthread::thread *myThread;
    bool isDone; //no mutex, because bool is atomic
    lightThread(lightThreadDispatch& dispatch);
    ~lightThread();
    void run();
private:
    void doLight(int x,int y);
    void doRay(const rgbf& power,int cx,int cy,int tx,int ty,int num_diffuse);
    rgbf lightUpCell(rgbf power,int dx,int dy,int tx,int ty);
};
=======
>>>>>>> 2779290b70feebb0fab5bd7225a18604efaf5cc9
class lightingEngineViewscreen:public lightingEngine
{
public:
    lightingEngineViewscreen(renderer_light* target);
	~lightingEngineViewscreen();
    void reinit();
    void calculate();

    void updateWindow();
<<<<<<< HEAD
    void preRender();
=======

>>>>>>> 2779290b70feebb0fab5bd7225a18604efaf5cc9
    void loadSettings();
    void clear();

    void debug(bool enable){doDebug=enable;};
private:
<<<<<<< HEAD
    void fixAdvMode(int mode);
    df::coord2d worldToViewportCoord(const df::coord2d& in,const DFHack::rect2d& r,const df::coord2d& window2d) ;
    

    void doSun(const lightSource& sky,MapExtras::MapCache& map);
    void doOcupancyAndLights();
    rgbf propogateSun(MapExtras::Block* b, int x,int y,const rgbf& in,bool lastLevel);
    void doRay(std::vector<rgbf> & target, rgbf power,int cx,int cy,int tx,int ty);
    void doFovs();
	void doLight(std::vector<rgbf> & target, int index);
    rgbf lightUpCell(std::vector<rgbf> & target, rgbf power,int dx,int dy,int tx,int ty);
    bool addLight(int tileId,const lightSource& light);
    void addOclusion(int tileId,const rgbf& c,float thickness);

    matLightDef* getMaterialDef(int matType,int matIndex);
    buildingLightDef* getBuildingDef(df::building* bld);
    creatureLightDef* getCreatureDef(df::unit* u);
    itemLightDef* getItemDef(df::item* it);

=======

    df::coord2d worldToViewportCoord(const df::coord2d& in,const DFHack::rect2d& r,const df::coord2d& window2d) ;
    bool isInViewport(const df::coord2d& in,const DFHack::rect2d& r);

    void doSun(const lightSource& sky,MapExtras::MapCache& map);
    void doOcupancyAndLights();
    lightCell propogateSun(MapExtras::Block* b, int x,int y,const lightCell& in,bool lastLevel);
    void doRay(std::vector<lightCell> & target, lightCell power,int cx,int cy,int tx,int ty);
    void doFovs();
	void doLight(std::vector<lightCell> & target, int index);
    lightCell lightUpCell(std::vector<lightCell> & target, lightCell power,int dx,int dy,int tx,int ty);
    bool addLight(int tileId,const lightSource& light);
    void addOclusion(int tileId,const lightCell& c,float thickness);

    matLightDef* getMaterial(int matType,int matIndex);
    buildingLightDef* getBuilding(df::building* bld);
    
>>>>>>> 2779290b70feebb0fab5bd7225a18604efaf5cc9
    //apply material to cell
    void applyMaterial(int tileId,const matLightDef& mat,float size=1, float thickness = 1);
    //try to find and apply material, if failed return false, and if def!=null then apply def.
    bool applyMaterial(int tileId,int matType,int matIndex,float size=1,float thickness = 1,const matLightDef* def=NULL);
    
    size_t inline getIndex(int x,int y)
    {
        return x*h+y;
    }
    df::coord2d inline getCoords(int index)
    {
        return df::coord2d(index/h, index%h);
    }
    //maps
<<<<<<< HEAD
    std::vector<rgbf> lightMap;
    std::vector<rgbf> ocupancy;
    std::vector<lightSource> lights;

    //Threading stuff
    int num_diffuse; //under same lock as ocupancy
    lightThreadDispatch threading;
    //misc
    void setHour(float h){dayHour=h;};

    int getW()const {return w;}
    int getH()const {return h;}
public:
	void lightWorkerThread(void * arg);
private:
    rgbf getSkyColor(float v);
=======
    std::vector<lightCell> lightMap;
    std::vector<lightCell> ocupancy;
    std::vector<lightSource> lights;

    //Threading stuff
    tthread::mutex indexMutex;
    tthread::mutex writeMutex;
    int nextIndex;
    std::vector<tthread::thread *> threadList;
    void doLightThreads();
    //misc
    void setHour(float h){dayHour=h;};
public:
	void lightWorkerThread(void * arg);
private:
    lightCell getSkyColor(float v);
>>>>>>> 2779290b70feebb0fab5bd7225a18604efaf5cc9
    bool doDebug;

    //settings
    float daySpeed;
    float dayHour; //<0 to cycle
<<<<<<< HEAD
    std::vector<rgbf> dayColors; // a gradient of colors, first to 0, last to 24
=======
    std::vector<lightCell> dayColors; // a gradient of colors, first to 0, last to 24
>>>>>>> 2779290b70feebb0fab5bd7225a18604efaf5cc9
    ///set up sane settings if setting file does not exist.
    void defaultSettings(); 

    static int parseMaterials(lua_State* L);
    static int parseSpecial(lua_State* L);
    static int parseBuildings(lua_State* L);
<<<<<<< HEAD
    static int parseItems(lua_State* L);
    static int parseCreatures(lua_State* L);
=======
>>>>>>> 2779290b70feebb0fab5bd7225a18604efaf5cc9
    //special stuff
    matLightDef matLava;
    matLightDef matIce;
    matLightDef matAmbience;
    matLightDef matCursor;
    matLightDef matWall;
    matLightDef matWater;
    matLightDef matCitizen;
    float levelDim;
<<<<<<< HEAD
    int adv_mode;
    //materials
    std::unordered_map<std::pair<int,int>,matLightDef> matDefs;
    //buildings
    std::unordered_map<std::tuple<int,int,int>,buildingLightDef> buildingDefs;
    //creatures
    std::unordered_map<std::pair<int,int>,creatureLightDef> creatureDefs;
    //items
    std::unordered_map<std::pair<int,int>,itemLightDef> itemDefs;
    int w,h;
    DFHack::rect2d mapPort;
    friend class lightThreadDispatch;
};
rgbf blend(const rgbf& a,const rgbf& b);
rgbf blendMax(const rgbf& a,const rgbf& b);
=======
    //materials
    std::map<std::pair<int,int>,matLightDef> matDefs;
    //buildings
    std::map<std::tuple<int,int,int>,buildingLightDef> buildingDefs;
    int w,h;
    DFHack::rect2d mapPort;
};
lightCell blend(lightCell a,lightCell b);
lightCell blendMax(lightCell a,lightCell b);
>>>>>>> 2779290b70feebb0fab5bd7225a18604efaf5cc9
#endif
