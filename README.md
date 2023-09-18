# FF_PugiXml
 
# USED LIBRARY
https://github.com/zeux/pugixml

# USAGE
* Create a C++ project.
* Create a folder which named "Plugins"
* Copy plugin to that folder. Name should be FF_PugiXml.
* Delete Binaries, Intermediate and Saved folder of your project.
* Right click to your .uproject and select "Generate Visual Studio Project Files"
* Open your project's .sln file
* Press F5. It will build plugin.

# NOTE
* We don't use Rider. So, we can't help you if you have a problem about adding external plugins to your build list.
* If there is a problem about out PugiXml implementation, feel free to give hint or even pull request.
* We can't help if you have installation problems. Because our plugin works on all of our computers in our company and it doesn't have any dependency or tricky setup.
* If you see weird "variable is not valid" like problems and variables have underlines, it means you couldn't install plugin, correctly.
* Look at plugin's "Content" folder before using it. It has a "BP_Template_PugiXml" actor and it contais sample mechanics.