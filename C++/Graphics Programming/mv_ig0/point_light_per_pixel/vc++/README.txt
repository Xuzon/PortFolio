
	Visual C++ Project Files

	The VC++ projects should compile without having to edit any project settings. However, when you attempt to run
	the program from inside the IDE it may not be able to locate the resources (textures etc.). To fix this you need to set the "Working Directory"
	setting (in the project properties) to "$(SolutionDir)..\". The working directory is a per-user setting, so it is not saved 
	in the *.vcproj file - this is why it doesn't work 'out-of-the-box'. Running the program from outside the IDE should always
	work. - Luke Benstead