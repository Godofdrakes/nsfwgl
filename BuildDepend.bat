Devenv nsfwgl.sln

cmake -DOPTION_BUILD_STATIC=ON -B./dep/glbinding/ -H./dep/glbinding/
cmake -B./dep/glfw/ -H./dep/glfw/

7z e ./dep/FBXFile.zip -o./dep/FBXFile/ -y

Devenv ./dep/glbinding/glbinding.sln /Build Debug /project ALL_BUILD
Devenv ./dep/glbinding/glbinding.sln /Build Release /project ALL_BUILD

Devenv ./dep/glfw/glfw.sln /Build Debug /project ALL_BUILD
Devenv ./dep/glfw/glfw.sln /Build Release /project ALL_BUILD

pause
