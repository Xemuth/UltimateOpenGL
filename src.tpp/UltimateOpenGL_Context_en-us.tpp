topic "class UltimateOpenGL_Context";
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[b42;2 $$4,4#13035079074754324216151401829390:normal]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[H6;0 $$6,0#05600065144404261032431302351956:begin]
[H6;0 $$7,7#E926FC203CAA1D9285856B78519401AA:code]
[ $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 UltimateOpenGL`_Context]]}}&]
[s3;%% &]
[s1;:String`:`:class: [@(0.0.255) class]_[* UltimateOpenGL`_Context]&]
[s4;%% [%- UltimateOpenGL`_Context][%-*  ]is the main class of UltimateOpenGL, 
it serve to carry Scene object (wich will carry Camera and GameObject) 
and Material object (wich representing different type of texture 
and colors). Futhermore, it also provide different method to 
controle the global time or delta time (time for one frame) of 
your application.&]
[s4;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor Detail]]}}&]
[s3; &]
[s5;:UltimateOpenGL`_Context`:`:UltimateOpenGL`_Context`(`): [* UltimateOpenGL`_Context
]()&]
[s2;%% Default constructor. Constructs an empty UltimateOpenGL`_context&]
[s3; &]
[s6; &]
[s5;:UltimateOpenGL`_Context`:`:UltimateOpenGL`_Context`(float`,float`): [* UltimateOpe
nGL`_Context]([@(0.0.255) float ][*@3 ScreenHeight], [@(0.0.255) float 
][*@3 ScreenWidht])&]
[s2;%% Constructs an empty UltimateOpenGL`_context and set screen 
size of OpenGL context.&]
[s3;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s3; &]
[s5;:UltimateOpenGL`_Context`:`:GetEllapsedTime`(`): [@(0.0.255) double]_[* GetEllapsedTi
me]()&]
[s2;%% Return the value in millisecond of elapsed time since start 
application (or Counter reset) as of occured.&]
[s6; &]
[s5;:UltimateOpenGL`_Context`:`:GetDeltaTime`(`): [@(0.0.255) double]_[* GetDeltaTime]()&]
[s2;%% Return the deltaTime. DeltaTime represent time it take in 
millisecond to generate/process one Frame.&]
[s6; &]
[s5;:UltimateOpenGL`_Context`:`:GetFPS`(`): [@(0.0.255) int]_[* GetFPS]()&]
[s2;%% Return the number of frame per second&]
[s6; &]
[s5;:UltimateOpenGL`_Context`:`:SetScreenSize`(float`,float`): [@(0.0.255) UltimateOpen
GL`_Context][@N `&]_[* SetScreenSize(][@(0.0.255) float ]ScreenHeight[* ,][@(0.0.255) float
 ]ScreenWidth[* )]&]
[s5;O_;%% Set Screen size of OpenGL context.&]
[s6;%% &]
[s5;:UltimateOpenGL`_Context`:`:CreateScene`(const Upp`:`:String`&`,Args`&`&`.`.`.args`): [@(0.0.255) t
emplate]_<[@(0.0.255) class...]_[*@4 Args]>_[_^Scene^ Scene][@(0.0.255) `&]_[* CreateScene](
[@(0.0.255) const]_[_^Upp`:`:String^ Upp`::String][@(0.0.255) `&]_[*@3 name], 
[*@4 Args][@(0.0.255) `&`&...]_args)&]
[s2;%% Create a scene, you must provide the Type of scene you want 
to create, If it`'s the first scene you created then it will 
be defined as the ActiveScene. :&]
[s2;%% &]
[s2;%% -|-|[1 Scene`& scene `= context.CreateScene<Scene>(`"MyFirstScene`");]&]
[s3;%% &]
[s6;%% &]
[s5;:UltimateOpenGL`_Context`:`:GetActiveScene`(`): [_^Scene^ Scene][@(0.0.255) `&]_[* GetA
ctiveScene]()&]
[s2;%% return the active Scene (by default the first created is the 
active one) if no scene have been created it will raise an [_@(255.42.0) assertion].
&]
[s3;%% &]
[s6;%% &]
[s5;:UltimateOpenGL`_Context`:`:GetScene`(const Upp`:`:String`&`): [_^Scene^ Scene][@(0.0.255) `&
]_[* GetScene]([@(0.0.255) const]_[_^Upp`:`:String^ Upp`::String][@(0.0.255) `&]_[*@3 name])
&]
[s2; [%% return scene named by ][*@3 name ][@(25) if no scene have been 
set, it will raise UOGLException.]&]
[s3;%% &]
[s6;%% &]
[s5;:UltimateOpenGL`_Context`:`:GetAllScene`(`): [_^Upp`:`:ArrayMap^ Upp`::ArrayMap]<[_^Upp`:`:String^ U
pp`::String],[_^Scene^ Scene]>`&_[* GetAllScene]()&]
[s2;%% return the vector map containing all scene.&]
[s3;%% &]
[s6;%% &]
[s5;:UltimateOpenGL`_Context`:`:SetActiveScene`(const Upp`:`:String`&`): [@(0.0.255) bo
ol]_[* SetActiveScene]([@(0.0.255) const]_[_^Upp`:`:String^ Upp`::String][@(0.0.255) `&]_[*@3 n
ame])&]
[s2; [%%  ]Set the Active scene.&]
[s3;%% &]
[s6;%% &]
[s5;:UltimateOpenGL`_Context`:`:RemoveScene`(const Upp`:`:String`&`): [@(0.0.255) bool]_
[* RemoveScene]([@(0.0.255) const]_[_^Upp`:`:String^ Upp`::String][@(0.0.255) `&]_[*@3 nameO
fScene])&]
[s2; Remove the scene. If the removed is the active scene, then active 
scene will be set to null (you must ensure to reset a new active 
scene else GetActive scene wich is called in lot of draw routine 
will raise an assertion).&]
[s3;%% &]
[s6;%% &]
[s5;:UltimateOpenGL`_Context`:`:CreateMaterial`(const Upp`:`:String`&`,Args`&`&`.`.`.args`): [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 T],[@(0.0.255) class...]_[*@4 Args]>_[*@4 T][@(0.0.255) `&]_
[* CreateMaterial]([@(0.0.255) const]_[_^Upp`:`:String^ Upp`::String][@(0.0.255) `&]_[*@3 `_
MaterialName], [*@4 Args][@(0.0.255) `&`&...]_args)&]
[s2; Create a new Material, you must provide the type of material 
you want to create. By default Two material are provided in UOGL 
: Color`_Material and Texture2D. To create a Texture2D by example, 
you must do:&]
[s0; &]
[s2; -|-|[1 Texture2D`& myTexture `= context.CreateMaterial<][%%1 Texture2D>][1 (`"sand`",`"p
ath of the texture`");]&]
[s3;%% &]
[s6;%% &]
[s5;:UltimateOpenGL`_Context`:`:AddMaterial`(const Upp`:`:String`&`,T`&`): [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 T]>_[*@4 T][@(0.0.255) `&]_[* AddMaterial]([@(0.0.255) cons
t]_[_^Upp`:`:String^ Upp`::String][@(0.0.255) `&]_[*@3 `_MaterialName],[*@4 T][@(0.0.255) `&
]_[*@3 MaterialToAdd])&]
[s2;%% Add a new material from an existing one, you must provide 
the type of material you want to copy :&]
[s0;%% &]
[s2;%% -|-|[1 Texture2D`& myTexture2 `= context.AddMaterial<Texture2D>(`"sandCopy`",myTex
ture);]&]
[s3;%% &]
[s6;%% &]
[s5;:UltimateOpenGL`_Context`:`:GetMaterial`(const Upp`:`:String`&`): [@(0.0.255) templ
ate]_<[@(0.0.255) class]_[*@4 T]>_[*@4 T][@(0.0.255) `&]_[* GetMaterial]([@(0.0.255) const]_[_^Upp`:`:String^ U
pp`::String][@(0.0.255) `&]_[*@3 `_MaterialName])&]
[s2;%% Return material. you must provide the type of material you 
want. Raise UoglException if materialName is unknow. Raise UoglException 
if cast of material have gone wrong.&]
[s2;%% &]
[s2;%% -|-|[1 Texture2D`& texture `= context.GetMaterial<Texture2D>(`"sand`");]&]
[s2;%% &]
[s6;%% &]
[s5;:UltimateOpenGL`_Context`:`:IsMaterialTypeOf`(const Upp`:`:String`&`): [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) bool]_[* IsMaterialTypeOf]([@(0.0.255) co
nst]_[_^Upp`:`:String^ Upp`::String][@(0.0.255) `&]_[*@3 `_MaterialName])&]
[s2;%% Return true if the type specified in template parameter are 
the same of the material looked for:&]
[s2;%% &]
[s0;%% -|-|[1 if(context.IsMaterialTypeOf<Texture2D>(`"sand`")) //Will 
return true]&]
[s0;%% &]
[s3;%% &]
[s6;%% &]
[s5;:UltimateOpenGL`_Context`:`:IsMaterialExist`(const Upp`:`:String`&`): [@(0.0.255) b
ool]_[* IsMaterialExist]([@(0.0.255) const]_[_^Upp`:`:String^ Upp`::String][@(0.0.255) `&]_
[*@3 `_MaterialName])&]
[s2;%% return true if the material looked for exist.&]
[s3;%% &]
[s6;%% &]
[s5;:UltimateOpenGL`_Context`:`:RemoveMaterial`(const Upp`:`:String`&`): [_^UltimateOpenGL`_Context^ U
ltimateOpenGL`_Context][@(0.0.255) `&]_[* RemoveMaterial]([@(0.0.255) const]_[_^Upp`:`:String^ U
pp`::String][@(0.0.255) `&]_[*@3 `_MaterialName])&]
[s2;%% Remove a material from context.&]
[s3;%% &]
[s6;%% &]
[s5;:UltimateOpenGL`_Context`:`:GetAllMaterials`(`): [_^Upp`:`:ArrayMap^ Upp`::ArrayMap]<
[_^Upp`:`:String^ Upp`::String],[_^Material^ Material]>`&_[* GetAllMaterials]()&]
[s2;%% return ArrayMap of every material.&]
[s3;%% &]
[s6;%% &]
[s5;:UltimateOpenGL`_Context`:`:Trace`(bool`): [@(0.0.255) void]_[* Trace]([@(0.0.255) bool
]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% Enable/Disable OpenGL logging&]
[s3;%% &]
[s6;%% &]
[s5;:UltimateOpenGL`_Context`:`:Draw`(const Upp`:`:String`&`,const Upp`:`:String`&`): [@(0.0.255) v
oid]_[* Draw]([@(0.0.255) const]_[_^Upp`:`:String^ Upp`::String][@(0.0.255) `&]_[*@3 SceneTo
Draw][@(0.0.255) `=]`"`",[@(0.0.255) const]_[_^Upp`:`:String^ Upp`::String][@(0.0.255) `&]_
[*@3 CameraToUse][@(0.0.255) `=]`"`")&]
[s2;%% Draw Active scene with Active camera unless you specify a 
SceneToDraw and a CameraToUse.&]
[s3;%% &]
[s6;%% &]
[s5;:UltimateOpenGL`_Context`:`:Initialise`(`): [@(0.0.255) void]_[* Initialise]()&]
[s2;%% Initialise OpenGL parameters like Blending (simulation of 
transparency) or Multisample. [@(255.42.0) Must be called before 
drawing anything].&]
[s3;%% &]
[s2;%% ]]