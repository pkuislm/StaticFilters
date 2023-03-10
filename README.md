# StaticFilters

Some moded static-linked DirectShow filters 

静态链接的LAVSplitter、LAVVideoDecoder、LAVAudioDecoder、VSFilter 
 
稍微做了点改动： 

+ LAV系列默认开启所有格式的解码功能，不显示任务栏图标以及不向注册表写入、读取设置 
+ VSFilter也默认不显示任务栏图标，不向注册表写入或读取设置，同时增加从内存加载PGS字幕的功能 

### 其他

#### ffmpeg配置

```
--toolchain=msvc --extra-cflags="-DHAVE_UNISTD_H=0 -MT -D_WIN32_WINNT=0x0600 -DWINVER=0x0600 -Zo -GS- -I../thirdparty/32/include/ " --disable-shared --enable-static --enable-gpl  --enable-version3 --disable-autodetect --enable-w32threads --enable-demuxer=asf --disable-demuxer=matroska --disable-filters --enable-filter=scale,yadif,w3fdif,bwdif --disable-protocol=async,cache,concat,httpproxy,icecast,md5,subfile --disable-muxers --enable-muxer=spdif --disable-bsfs --enable-bsf=extract_extradata --disable-avdevice --disable-postproc --disable-encoders --disable-devices --disable-programs --disable-doc --enable-avisynth --enable-d3d11va --enable-dxva2 --enable-schannel --enable-zlib --build-suffix=-lav --arch=x86 --disable-debug
```

#### VSFilter说明  

因为是加了点东西，所以有新的GUID。VSFilter原有的功能不受影响。 

用`CLSID_DirectVobSubMod`和`IID_IBaseFilter`拿到Filter，然后用`IID_IDirectPGS`拿到接口，调用里面的`SetPGSAndLoadImmediately`就行。将这个Filter连到`VideoDecoder`和`Renderer`中间就行，或者你可以只连`VideoDecoder`，然后调用`IGraphBuilder`的`Render`让他自己连接下游。要用到的东西我放在下面： 

```
DEFINE_GUID(CLSID_DirectVobSubMod, 0x93A22E7A, 0x5091, 0x45EF, 0xBA, 0x61, 0x6D, 0xA2, 0x61, 0x56, 0xA5, 0xD1);

interface __declspec(uuid("AB52FC9C-2415-4dca-BC1C-8DCC2EAE8151"))
IDirectPGS :
public IUnknown {
    STDMETHOD(SetPGSAndLoadImmediately) (BYTE* buf, int bufSize) PURE;
};
```
 
LAV的那个就是注释掉点代码，然后写死点设置，没啥看头。VSFilter里我动过的代码待会扔上来（虽然也没动多少） 
 
就酱。
