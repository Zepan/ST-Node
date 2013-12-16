简介
====
bootDownloader是一个通过swim接口向没有hairBoot的芯片中烧写hairBoot的应用程序demo。
该应用运行在已烧录hairBoot的ST-Node中，它会将当前芯片flash中的hairBoot通过swim接口烧录至新的芯片中。

使用方法
========
1. 将该应用源码通过STM8 Launcher编译并下载至ST-Node
2. 应用中的引脚设定为：PC7,Vcc(可选，可以使用3.3V替代); PC6,SWIM; PC5,RST; PC4,指示LED(可选,接阳极); 以及UART。  
    按照对应关系连接至待烧写芯片的Vcc, Gnd, SWIM, RST引脚， uart连接至PC串口（可选，只是为了查看烧写结果）
3. 给ST-Node上电，然后观察串口输出的提示信息或者led的指示；正常的话应该上电瞬间就提示烧写成功，led常亮。
4. 烧录完成！ 新片已经可以使用STM8 Launcher进行串口下载了~
