# Convert utf8 to gb2312

sourceinsight是windows上少有的优秀源码阅读工具，由于linux文件经常使用utf-8编码，而sourceinsight并不支持utf-8，对于英文字母，各种编码存储无差别，而中文因为编码不同，存储方式各异，使用不正确的编码方式打开时便无法识别，出现中文乱码。本工程实现转码功能，将utf-8编码文件转换为Windows平台常见的gb2312编码文件，解决sourceinsight中文乱码问题。

### Instructions：
> 1. 可配置仅转换当前目录文件或者级联转换所有子目录的文件。
> 2. 默认转换 h,c,cpp,cxx,hpp,php,java,txt
常见的文件类型。可以配置转换其中的一种或者几种，此时默认配置失效。当自定义配置的文件类型不再这几种之中时，不进行转换。

### Notes：
> 1. 使用vs2013可直接打开工程，若版本较低新建工程后直接copy添加源文件即可。
> 2. 由于使用了windows平台的struct _finddata_t结构，故需要在vc环境下编译。

### Updates：
> 1. 增加弹出对话框选择转换的目录。自定义默认转换文件类型仍需在main函数设置。
