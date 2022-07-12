# 具体设计

我们的公交车调度系统——OJ版将功能实现划分为了命令读取，任务判断等不同的模块，并且设置了一些全局变量用于代表配置和车辆的状态以及储存任务等，下面将从变量和函数具体阐明。

##　变量

### 配置变量

`a_TotalStation` `a_Strategy` `a_Distance` `distance` `strategy` `totalStation` `tatalDistance`

以上的变量分别如其英文名所示，存储了站点数，站点间距离，总距离，调度策略等配置信息，其中前缀为`a_`是由配置读取函数`initConfigReader()`控制，且设置有初始值，并通过对应的三个`get?()`函数返回，这些变量主要由函数`getNextTask()` `fulfillQuery()` 使用，作为获取下一个任务以及完成当前任务的依据

### 公交车状态变量

`nowDirection` `busPosition` `isInCharge` `stationGoto` `nowDirection`

以上变量分别代表公交车当前方向，当前位置，是否处于执行任务状态，去往的站点以及当前方向，这些变量主要用于在接受到时间信号后根据策略进行判断，是否需要完成任务，抑或是前进，主要用于在接收到`clock`信号后作为判断依据

### 任务相关变量

`queryList[10001]` `queryNumber` `isCounterclockwiseQuery[1001]`  `isClockwiseQuery[1001]` `isBusQuery[1001]` 

第一个数组的作用是储存读取到的任务请求，类型是Query，我们设计的结构体，包含了任务的类型，出现时间，以及目标站点，上一个任务完成之后会由函数`getNextTask()` 从该数组中判断出正确的下一个任务；`queryNumber`则表示的是目前存在未完成的任务个数。后面三个数组分别代表每个站点是否存在三种类型的请求，是则为1，否则为0，用于输出，且根据`queryList`中存在的任务进行及时更新

###　其他

`nowClock` 代表当前时间，每次接收到时钟信号`clock`后自增

`flagFirstTime` 用于MODE_SCAN第一次任务确定方向标志位

## 模块函数

### 配置获取模块

`initConfigReader()` 打开配置文件`dict.dic`读取其中的配置信息，将读取到的配置信息储存到`a_TotalStation` `a_Strategy` `a_Distance` 三个变量中；并将三个变量值的获取分别封装到三个函数`getTotalStation()` `getDistance()` `getStrategy()`中

### 任务读取模块

`getCommand()`用于再循环之初读取命令，首先对命令进行判断：

若为`end`则直接退出循环（退出循环的唯一方式）

若为`clock`即时钟信号，则将`nowClock` +1，代表时间增长，并返回`CLOCK` 进入循环中的任务处理部分

若为三种请求信号，则首先根据请求出现的站点，时间，以及类型，遍历任务列表，判断是否存在相同任务，若存在，则无视，若不存在，则添加到任务列表`queryList[10001]`中，同时修改对应的输出列表例如`isClockwiseQuery[1001]`

### 任务处理模块

`int getNextTask(Query queryList[],int queryListNumber, int direction,int strategyType,int totalStation,int distance,int nowStation)` 在上一个任务完成后再次接收到时钟信号，且任务列表不为空的情况下调用此函数，这个函数也是调度方案的主要实现函数，即确定下一个任务，并返回其在数组中的下标。

函数首先判断公交调度的策略`strategyType`：

若为`FCFS`，则根据先来先服务的原则，直接返回下标1，代表最早进入任务列表的任务；

若为`SSTF`， 则根据车辆当前位置`nowStation`，对传进来的任务列表进行遍历，分别计算从当前位置到任务列表中每一个任务的顺逆时针距离，分别得出最短的顺逆时针任务，结合当前车辆的行驶方向`direction`,再次比较后确定最优方案，并返回其下标

若为`SCAN`，第一次寻径原则与`SSTF`一致，之后的寻径原则与SSTF的不同点在于只考虑当前方向下的最短时间（路径）



`int fulfillQuery(Query queryList[],int *queryListNumber,int direction,int strategyType,int totalStation,int distance,int nowStation)`这个函数用于解决顺便服务，调用的原则是，策略为`FCFS` 或`SCAN`，在出现时钟信号后，车辆移动到非终点站的站点。函数内首先会判断`strategyType`，再对任务数组进行遍历，判断是否有可以执行的顺便服务请求，若可执行，则删去，若不可执行，则保留并复制到临时的任务列表中，最后再将临时的任务列表拷贝回原任务列表，实现任务的删除与重排，最后返回被删除的任务个数。

### 输出模块

`updateOutputList()`这个函数用于控制 `isCounterclockwiseQuery[1001]`  `isClockwiseQuery[1001]` `isBusQuery[1001]` 三个输出列表，在每次完成任务（包括站内完成，到达终点站，顺便服务）后调用，根据最新的任务列表修正输出列表中的站点是否存在请求



`outputResult()`再接收到时钟信号之后，处理完所有逻辑判断，进入下一个循环之前根据三个输出列表进行一次状态输出



### 其他

`moveForward()` 前进函数，接收到时钟信号后，在没有可以完成的请求的情况下，根据前进方向位置加或减1，利用对总距离取余的方法实现路径的循环

`getStationPosition()` `getPositionStation()`根据公交车站点获取位置，根据公交车位置获取站点

`getClockwiseDist()` `getCounterClockwiseDist()` 获取公交车当前位置去往目标站点的顺\逆时针距离

`chooseDirection()` 先来先服务，最短时间服务确定方向

`keepDirectionValid()`顺便服务，检查是否需要变向

`firstTimeScanGetDir()` SCAN模式第一次找任务



## 主函数逻辑

在进入第一次主函数之后，要执行一次配置模块，之后进入`while`循环，往复的读取命令，其中只有读到`clock`命令后开始处理任务，通过对车辆的位置等信息进行判断是前进还是完成任务，抑或是根据策略可以进行顺便服务，每一个`clock`循环的末尾都要进行一次输出；读到`end`命令直接退出循环；读到三种请求的命令则对任务列表和输出列表进行修改.















