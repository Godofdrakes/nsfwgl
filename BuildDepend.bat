cmake -DOPTION_BUILD_STATIC=ON -B./dep/glbinding/ -H./dep/glbinding/
Devenv ./dep/glbinding/glbinding.sln /Build Debug /project ALL_BUILD
Devenv ./dep/glbinding/glbinding.sln /Build Release /project ALL_BUILD

cmake -B./dep/glfw/ -H./dep/glfw/
Devenv ./dep/glfw/glfw.sln /Build Debug /project ALL_BUILD
Devenv ./dep/glfw/glfw.sln /Build Release /project ALL_BUILD

"./dep/7z.exe" e ./dep/FBXFile.zip -o./dep/FBXFile/

pause
