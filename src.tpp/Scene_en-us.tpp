topic "class Scene";
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[b42;2 $$4,4#13035079074754324216151401829390:normal]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[H6;0 $$6,0#05600065144404261032431302351956:begin]
[ $$0,0#00000000000000000000000000000000:Default]
[{_} 
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 Scene]]}}&]
[s3;%% &]
[s1;:String`:`:class: [@(0.0.255) class]_[* Scene]&]
[s4; Scene is a way of representing a  real scene (see it as a place 
where we can have several object 3D/ Light / sound emitor) and 
many other things. A scene can carry differents camera and gameobject 
type. It also carry a Color`_Material object named Skybox wich 
allow to set color of background (real skybox will be implemented 
in futurs).While drawing process, Draw method of scene will be 
called. While loading process, Load method will be called. Many 
of GamesObject will requiere access to context wich mean you 
must ensure the scene have a valid context UOGL`_context pointers 
stored in context variable. This last point wont be a problem 
is you always create your scene using the method provided by 
UOGL`_context object ([_^Scene^ Scene][@(0.0.255) `&]_[* CreateScene]([@(0.0.255) const]_
[_^Upp`:`:String^ Upp`::String][@(0.0.255) `&]_[*@3 name], [*@4 Args][@(0.0.255) `&`&...]_ar
gs)).&]
[s4;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor Detail]]}}&]
[s3; &]
[s6; &]
[s5;:Scene`:`:Scene`(`): [* Scene]()&]
[s2;%% &]
[s3; &]
[s6; &]
[s5;:Scene`:`:Scene`(UltimateOpenGL`_Context`&`): [* Scene]([_^UltimateOpenGL`_Context^ U
ltimateOpenGL`_Context][@(0.0.255) `&]_[*@3 `_context])&]
[s2;%%  [%-*@3 `_context] .&]
[s3;%% &]
[s6;%% &]
[s5;:Scene`:`:Scene`(UltimateOpenGL`_Context`&`,const Upp`:`:String`&`): [* Scene]([_^UltimateOpenGL`_Context^ U
ltimateOpenGL`_Context][@(0.0.255) `&]_[*@3 `_context], [@(0.0.255) const]_[_^Upp`:`:String^ U
pp`::String][@(0.0.255) `&]_[*@3 `_name])&]
[s2;%%  [%-*@3 `_context] [%-*@3 `_name] .&]
[s3;%% &]
[s6;%% &]
[s5;:Scene`:`:Scene`(Scene`&`): [* Scene]([* Scene][@(0.0.255) `&]_[*@3 `_scene])&]
[s2;%%  [%-*@3 `_scene] .&]
[s3;%% &]
[s5;:UltimateOpenGL`_Context`:`:UltimateOpenGL`_Context`(`):* &]
[s3;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s6; &]
[s5;:UltimateOpenGL`_Context`:`:GetScene`(const Upp`:`:String`&`): [@(0.0.255) bool]_[* I
sCameraExist]([@(0.0.255) const]_[_^Upp`:`:String^ Upp`::String][@(0.0.255) `&]_[*@3 `_Cam
eraName])&]
[s2;%%  [%-*@3 `_CameraName] .&]
[s3;%% &]
[s6;%% &]
[s5;:Scene`:`:IsLightExist`(const Upp`:`:String`&`): [@(0.0.255) bool]_[* IsLightExist]([@(0.0.255) c
onst]_[_^Upp`:`:String^ Upp`::String][@(0.0.255) `&]_[*@3 `_LightName])&]
[s2;%%  [%-*@3 `_LightName] .&]
[s3;%% &]
[s6;%% &]
[s5;:Scene`:`:IsGameObjectExist`(const Upp`:`:String`&`): [@(0.0.255) bool]_[* IsGameObje
ctExist]([@(0.0.255) const]_[_^Upp`:`:String^ Upp`::String][@(0.0.255) `&]_[*@3 `_ObjectNa
me])&]
[s2;%%  [%-*@3 `_ObjectName] .&]
[s3;%% &]
[s6;%% &]
[s5;:Scene`:`:operator`=`(Scene`&`): [_^Scene^ Scene][@(0.0.255) `&]_[* operator`=]_([_^Scene^ S
cene][@(0.0.255) `&]_[*@3 `_scene])&]
[s2;%%  [%-*@3 `_scene] .&]
[s3;%% &]
[s6;%% &]
[s5;:Scene`:`:GetContext`(`): [_^UltimateOpenGL`_Context^ UltimateOpenGL`_Context][@(0.0.255) `&
]_[* GetContext]()&]
[s2;%% &]
[s3;%% &]
[s6;%% &]
[s5;:Scene`:`:GetName`(`): [_^Upp`:`:String^ Upp`::String]_[* GetName]()&]
[s2;%% &]
[s3;%% &]
[s6;%% &]
[s5;:Scene`:`:GetSkyBox`(`): [_^Color`_Material^ Color`_Material][@(0.0.255) `&]_[* GetSkyB
ox]()&]
[s2;%% &]
[s3;%% &]
[s6;%% &]
[s5;:Scene`:`:GetAllGameObjects`(`): [_^Upp`:`:ArrayMap^ Upp`::ArrayMap]<[_^Upp`:`:String^ U
pp`::String],[_^GameObject^ GameObject]>`&_[* GetAllGameObjects]()&]
[s2;%% &]
[s3;%% &]
[s6;%% &]
[s5;:Scene`:`:GetAllCameras`(`): [_^Upp`:`:ArrayMap^ Upp`::ArrayMap]<[_^Upp`:`:String^ Upp
`::String],[_^Camera^ Camera]>`&_[* GetAllCameras]()&]
[s2;%% &]
[s3;%% &]
[s6;%% &]
[s5;:Scene`:`:GetAllLights`(`): [_^Upp`:`:ArrayMap^ Upp`::ArrayMap]<[_^Upp`:`:String^ Upp`:
:String],[_^Light^ Light]>`&_[* GetAllLights]()&]
[s2;%% &]
[s3;%% &]
[s6;%% &]
[s5;:Scene`:`:SetContext`(UltimateOpenGL`_Context`&`): [_^Scene^ Scene][@(0.0.255) `&]_[* S
etContext]([_^UltimateOpenGL`_Context^ UltimateOpenGL`_Context][@(0.0.255) `&]_[*@3 `_c
ontext])&]
[s2;%%  [%-*@3 `_context] .&]
[s3;%% &]
[s6;%% &]
[s5;:Scene`:`:SetName`(const Upp`:`:String`&`): [_^Scene^ Scene][@(0.0.255) `&]_[* SetName](
[@(0.0.255) const]_[_^Upp`:`:String^ Upp`::String][@(0.0.255) `&]_[*@3 `_name])&]
[s2;%%  [%-*@3 `_name] .&]
[s3;%% &]
[s6;%% &]
[s5;:Scene`:`:SetSkyBox`(Color`_Material`&`): [_^Scene^ Scene][@(0.0.255) `&]_[* SetSkyBox](
[_^Color`_Material^ Color`_Material][@(0.0.255) `&]_[*@3 `_skyBox])&]
[s2;%%  [%-*@3 `_skyBox] .&]
[s3;%% &]
[s6;%% &]
[s5;:Scene`:`:RemoveCamera`(const Upp`:`:String`&`): [_^Scene^ Scene][@(0.0.255) `&]_[* Rem
oveCamera]([@(0.0.255) const]_[_^Upp`:`:String^ Upp`::String][@(0.0.255) `&]_[*@3 `_Camera
Name])&]
[s2;%%  [%-*@3 `_CameraName] .&]
[s3;%% &]
[s6;%% &]
[s5;:Scene`:`:SetActiveCamera`(const Upp`:`:String`&`): [_^Scene^ Scene][@(0.0.255) `&]_[* S
etActiveCamera]([@(0.0.255) const]_[_^Upp`:`:String^ Upp`::String][@(0.0.255) `&]_[*@3 `_C
ameraName])&]
[s2;%%  [%-*@3 `_CameraName] .&]
[s3;%% &]
[s6;%% &]
[s5;:Scene`:`:GetActiveCamera`(`): [_^Camera^ Camera][@(0.0.255) `&]_[* GetActiveCamera]()&]
[s2;%% &]
[s3;%% &]
[s6;%% &]
[s5;:Scene`:`:RemoveGameObject`(const Upp`:`:String`&`): [_^Scene^ Scene][@(0.0.255) `&]_
[* RemoveGameObject]([@(0.0.255) const]_[_^Upp`:`:String^ Upp`::String][@(0.0.255) `&]_[*@3 `_
ObjectName])&]
[s2;%%  [%-*@3 `_ObjectName] .&]
[s3;%% &]
[s6;%% &]
[s5;:Scene`:`:RemoveLight`(const Upp`:`:String`&`): [_^Scene^ Scene][@(0.0.255) `&]_[* Remo
veLight]([@(0.0.255) const]_[_^Upp`:`:String^ Upp`::String][@(0.0.255) `&]_[*@3 `_LightNam
e])&]
[s2;%%  [%-*@3 `_LightName] .&]
[s3;%% &]
[s6;%% &]
[s5;:Scene`:`:Load`(`): [_^Scene^ Scene][@(0.0.255) `&]_[* Load]()&]
[s2;%% &]
[s3;%% &]
[s6;%% &]
[s5;:Scene`:`:IsLoaded`(`): [@(0.0.255) bool]_[* IsLoaded]()&]
[s2;%% &]
[s3;%% &]
[s6;%% &]
[s5;:Scene`:`:Draw`(const Upp`:`:String`&`): [_^Scene^ Scene][@(0.0.255) `&]_[* Draw]([@(0.0.255) c
onst]_[_^Upp`:`:String^ Upp`::String][@(0.0.255) `&]_[*@3 CameraToUse]_`=_`"`")&]
[s2;%%  [%-*@3 CameraToUse] .&]
[s3;%% &]
[s3; ]]