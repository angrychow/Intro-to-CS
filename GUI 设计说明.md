### GUI 设计说明

GUI 大概分成两块，一块是主页面，一块是新增需求页面

#### 主页面

主页面分上下两块，上面那一块显示当前车辆位置、车辆行进状态（逆时针、顺时针、到站）

主页面下方有一个列表，罗列当前所有请求

列表下方有按钮若干：

+ 按钮一：刷新请求列表
+ 按钮二：时钟下拨一秒
+ 按钮三：添加新请求

点击按钮三新增窗口实例 —— 添加请求页面

#### 添加请求页面

添加请求页面是一个小 Widget

一个输入框，输入站点

一个多选框，选择请求类型

一个按钮，点击提交请求

点击提交到调用新增请求函数前，页面会对输入内容合法性进行检测，不合法不会提交，合法提交后销毁页面实例