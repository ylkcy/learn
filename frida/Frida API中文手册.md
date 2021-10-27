
# Frida API中文手册

## Frida

- `Frida.version` Frida版本信息 String
- `Frida.heapsize` Frida私有堆的大小 Number

## Script

- `Script.runtime` Frida运行时间, 不包含QJS和V8

## Thread

- `Thread.backtrace([context, backtracer])` 抓取当前线程的调用堆栈，并以 NativePointer 指针数组的形式返回。如果你是在 Interceptor.onEnter或者Interceptor.onLeave 中调用这个函数的话，那就必须要把 this.context 作为参数传入，这样就能拿到更佳精准的堆栈调用信息，如果省略这个参数不传，那就意味着从当前堆栈的位置开始抓取，这样的抓取效果可能不会很好，因为有不少V8引擎的栈帧的干扰。第二个可选参数 backtracer，表示使用哪种类型的堆栈抓取算法，目前的取值范围是 Backtracer.FUZZY 和 Backtracer.ACCURATE，目前后者是默认模式。精确抓取模式下，如果程序是调试器友好（比如是标准编译器编译的结果，没有什么反调试）或者有符号表的支持，抓取效果是最好的，而模糊抓取模式下，抓取器会在堆栈上尝试抓取，并且会猜测里面包含的返回地址，也就是说中间可能包含一些错误的信息，但是这种模式基本能在任何二进制程序里面工作：

    ```javascript
    const f = Module.getExportByName('libcommonCrypto.dylib', 'CCCryptorCreate');
    Interceptor.attach(f, {
        onEnter(args) {
            console.log('CCCryptorCreate called from:\n' +
                Thread.backtrace(this.context, Backtracer.ACCURATE)
                .map(DebugSymbol.fromAddress).join('\n') + '\n');
        }
    });
    ```

- `Thread.sleep(delay)` 挂起当前线程的执行，延时delay秒后继续执行

### Process

- `Process.id` 当前进程ID Number
- `Process.arch` 处理器类型 String (ia32 | X64 | arm | arm64)
- `Process.platform` 操作系统类型 String (windows | linux | darwin | qnx)
- `Process.PageSize` 虚拟页面的大小 Number
- `Process.pointerSize` 指针类型的大小 Number
- `Process.codeSigningPolicy` 代码签名策略
- `Process.isDebuggerAttached()` 进程是否被调试 boolean
- `Process.getCurrentThreadId()` 当前线程ID Number
- `Process.enumerateThreads()` 枚举进程内所有线程 thread array objects
  - `thread.id` 线程id Number
  - `thread.state` 线程的状态 String (running | stopped | waiting | uninterruptible | halted) String
  - `thread.context` 线程上下文(EIP/RIP/PC and ESP/RSP/SP) NativePointer
- `Process.enumerateThreadsSync(callbacks)` Process.enumerateThreads(callbacks)同步版本
- `Process.findModuleByAddress(address) | Process.getModuleByAddress(address) | Process.findModuleByName(name) | Process.getModuleByName(name)` 获取地址对应的模块信息 Module 不存在该模块 find前缀会返回null get前缀会抛出异常
- `Process.enumerateModules()` 获取进程加载的所有模块 module array
  - `module.name` 模块名称
  - `module.base` 模块基址
  - `module.size` 模块大小
  - `module.path` 模块路径
- `Process.enumerateModulesSync()` enumerateModules()函数的同步版本，返回模块对象数组
- `Process.findRangeByAddress(address) | getRangeByAddress(address)` 返回有关包含地址的范围的信息 object, 对应字段参考
- `Process.enumerateRanges(protection|specifier)` 枚举指定类型protoction的内存块，以指定字符串给出"rwx", "rw-"标识至少可读可写，也可以使用分类符, 里面包含protection这个Key，取值就是前面提到的rwx，还有一个coalesce这个Key，表示是否要把位置相邻并且属性相同的内存块合并给出结果
  - `onMatch: function(range)` 每次枚举到一个内存块都回调回来，如果要停止枚举过程，直接返回stop即可.其中Range对象包含如下属性：
    - `range.base` 基地址
    - `range.size` 内存块大小
    - `range.protection` 保护属性
    - `range.file`（如果有的话）内存映射文件：
      - `file.path` 文件路径
      - `file.offset` 文件内偏移
  - `onComplete: function()` 所有内存块枚举完成之后会回调
- `Process.setExceptionHandler(callback)` 设置异常处理回调，其中回调函数有一个参数. 捕获到异常之后，怎么使用就看你自己了，比如可以把异常信息写到日志里面，然后发送个信息给主控端，然后同步等待主控端的响应之后处理，或者直接修改异常信息里面包含的寄存器的值，尝试恢复掉异常，继续执行。如果你处理了异常信息，那么这个异常回调里面你要返回 true，Frida会把异常交给进程异常处理函数处理，如果到最后都没人去处理这个异常，就直接结束目标进程。arg: details
  - `type` 异常类型 String (abort access-violation guard-page 非法访问内存| illegal-instruction 非法指令 | stack-overflow 栈溢出 | arithmetic 算术异常 |breakpoint 断点 |  single-step 单步 | system 系统错误)
  - `address` 异常地址 NativePointer
  - `memory`  如果这个对象非空 object containing:
  - `memory.operation` 触发异常的类型 String(read | write | execute)
  - `memory.address` 异常发生的地址 NativePointer
  - `context` 异常发生的线程上下文信息 NativePointer objects
      对象，其中，pc和sp是分别为ia32/x64/arm指定EIP/RIP/pc和ESP/RSP/sp的本地指针对象。其他处理器特定键也可用，例如eax、rax、r0、x0等。您也可以通过分配给这些键来更新寄存器值。
  - `nativeContext` NativePointer

  ``` shell
    ## 使用frida-ps 查找进程
    [root@yanglk fridastest]# frida-ps | grep main
    43280  main  
    ## 附加到指定进程           
    [root@yanglk fridastest]# frida -p 43280
     ____
      / _  |   Frida 15.1.8 - A world-class dynamic instrumentation toolkit
    | (_| |
      > _  |   Commands:
    /_/ |_|       help      -> Displays the help system
    . . . .       object?   -> Display information about 'object'
    . . . .       exit/quit -> Exit
    . . . .
    . . . .   More info at https://frida.re/docs/home/
    ## 查看处理器类型                                                                            
    [Local::PID::43280]-> Process.arch
    "x64"
    ## 查看进程ID
    [Local::PID::43280]-> Process.id
    43280
    ## 查看系统平台
    [Local::PID::43280]-> Process.platform
    "linux"
    ## 查看代码签名策略
    [Local::PID::43280]-> Process.codeSigningPolicy
    "optional"
    ##1 查看进程是否被调试
    [Local::PID::43280]-> Process.isDebuggerAttached()
    false
    ##2 使用gdb进行调试
    gdb -p 43280
    ##3 再次查看进程是否被调试
    [Local::PID::43280]-> Process.isDebuggerAttached()
    true
    /*
      function showThreads()
      {
        var thread_array = Process.enumerateThreads(
        );


        for(var i = 0; i < thread_array.length; i++)
        {
                console.log(JSON.stringify(thread_array[i]));
        }
      }
    */
    ## 枚举进程内部所有线程
    [Local::PID::43280]-> showThreads()
    {"id":43280,
     "state":"waiting",
     "context":
      {  
        "pc":"0x7fc5fed695a5",
        "sp":"0x7ffc22b00998",
        "rax":"0xfffffffffffffe00",
        "rcx":"0x7fc5fed695a5",
        "rdx":"0x400",
        "rbx":"0x7fc5ff03b9c0",
        "rsp":"0x7ffc22b00998",
        "rbp":"0xd68",
        "rsi":"0x16836b0",
        "rdi":"0x0",
        "r8":"0x7fc5ff03c6e0",
        "r9":"0x7fc5ff03d860",
        "r10":"0x7fc5ff463740",
        "r11":"0x246",
        "r12":"0x7fc5ff037880",
        "r13":"0x7fc5ff0383c0",
        "r14":"0x7fc5ff0383c0",
        "r15":"0x0",
        "rip":"0x7fc5fed695a5"
      }
    }
    /*
    function showModules()
    {
      var module_array = Process.enumerateModules();

      for(var i = 0; i < module_array.length; i++)
      {
        console.log(JSON.stringify(module_array[i]));
      }
    }
    */
    ## 枚举进程所有加载的模块
    [Local::PID::43280]-> showModules()
    {"name":"main","base":"0x400000","size":2105344,"path":"/root/hook/fridastest/main"}
    {"name":"linux-vdso.so.1","base":"0x7ffc22b08000","size":8192,"path":"linux-vdso.so.1"}
    {"name":"libtest.so","base":"0x7fc5ff041000","size":2105344,"path":"/root/hook/fridastest/libtest.so"}
    {"name":"libc-2.28.so","base":"0x7fc5fec7c000","size":3936256,"path":"/usr/lib64/libc-2.28.so"}
    {"name":"ld-2.28.so","base":"0x7fc5ff243000","size":2289664,"path":"/usr/lib64/ld-2.28.so"}
    {"name":"libpthread-2.28.so","base":"0x7fc5fea5c000","size":2211840,"path":"/usr/lib64/libpthread-2.28.so"}
    {"name":"libdl-2.28.so","base":"0x7fc5fc89b000","size":2113536,"path":"/usr/lib64/libdl-2.28.so"}
    {"name":"librt-2.28.so","base":"0x7fc5fc47c000","size":2129920,"path":"/usr/lib64/librt-2.28.so"}
    {"name":"frida-agent-64.so","base":"0x7fc5fca9f000","size":24530944,"path":"/tmp/frida-772678697214ec92c9ab393027cde22d/frida-agent-64.so"}
    {"name":"libresolv-2.28.so","base":"0x7fc5fc684000","size":2183168,"path":"/usr/lib64/libresolv-2.28.so"}
    {"name":"libm-2.28.so","base":"0x7fc5fc0fa000","size":3678208,"path":"/usr/lib64/libm-2.28.so"}
 
  ```

### Module

  Module.load() and Process.enumerateModules()等函数调用后返回Module对象，其中包含的属性

- `Module.name` 模块名称 String
- `Module.base` 模块基地址 NativePointer
- `Module.size` 模块大小 size in bytes
- `Module.path` 模块在文件系统中的完整路径 String
- `Module.enumerateImports()` 枚举模块的导入表信息, object array
  - `onMatch: function(imp)` 枚举到一个导入项到时候会被调用，imp包含如下的字段, 以下所有的属性字段，只有name字段是一定会有，剩余的其他字段不能保证都有，底层会尽量保证每个字段都能给出数据，但是不能保证一定能拿到数据，onMatch函数可以返回字符串stop表示要停止枚举。
    - `imp.type` 导入表中的类型 (function | variable) String
    - `imp.name`名称信息 String
    - `imp.module` 模块名称 String
    - `imp.address`绝对地址 NativePointer
    - `imp.slot` 导入标的入口地址 NativePointer
  - `onComplete: function()` 当所有的导入表项都枚举完成的时候会回调
- `Module.eumerateImportsSync(name)` enumerateImports()的同步版本
- `Module.enumerateExports(name, callbacks)` 枚举指定模块name的导出表项，结果用callbacks进行回调
  - `onMatch: function(exp)` 函数返回stop的时候表示停止枚举过程，其中exp代表枚举到的一个导出项，包含如下几个字段
    - `exp.type` 导出表中的类型 (function | variable) String
    - `exp.name` 名称信息 String
    - `exp.address` 绝对地址 NativePointer
  - `onComplete: function()` 枚举完成时回调
- `Module.enumerateExportsSync()` Module.enumerateExports()的同步版本
- `Module.enumerateSymbols(name, callbacks)` 枚举指定模块中包含的符号，枚举结果通过回调进行通知
  - `onMatch: function(sym)` 其中 sym 包含下面几个字段
    - `sym.isGlobal` 全局可见标识 boolean
    - `sym.type` 类型 (unknown | section | undefined(Mach-O) | absolute (Mach-O) | prebound-undefined (Mach-O) | indirect (Mach-O) | object (ELF)| function (ELF) | file   (ELF) | common (ELF) | tls (ELF)) String
    - `section`: 如果这个字段不为空的话，包含如下属性
    - `section.id` String containing section index, segment name (if applicable) and section name – same format as r2’s section IDs
    - `section.protection` 读写属性，类似"rwx"
    - `section.name` 符号名称 String
    - `section.address` 绝对地址 NativePointer
    - `section.size` if present, a Number specifying the symbol’s size in bytes
- `Module.enumerateSymbolsSync(name)` Module.enumerateSymbols() 的同步版本
  - `Module.enumerateRanges(name, protection, callbacks)` 功能基本等同于 Process.enumerateRanges(), 只不过多了一个模块名限定了枚举的范围
  - `Module.findExportByName(exportName)` 根据导出函数名称获取函数地址，如果不存在对应的导出函数，返回null
  - `Module.load(path)` 加载指定路径的模块, 如果加载失败，产生异常
  - `Module.ensureInitialized(name)` 确保指定模块的初始化程序已经运行。这在早期检测期间很重要, 即代码在进程生命周期的早期运行，以便能够安全地与 API交互
  - `Module.findBaseAddress(name)`获取模块基地址, 如果获取失败，产生异常
  - `Module.findExportByName(moduleName|null, exportName)`
  - `Module.getExportByName(moduleName|null, exportName)` 从指定模块中加载导出函数，如果模块名称未知，可以填null，但会花费时间去遍历所有模块，去查找对应的导出函数
### ModuleMap
  `new ModuleMap([filter])` 可以理解为内存模块快照，主要目的是可以作为一个模块速查表，比如你可以用这个快照来快速定位一个具体的地址是属于哪个模块。创建ModuleMap的时候，就是对目标进程当前加载的模块的信息作一个快照，后续想要更新这个快照信息的时候，可以使用 update 进行更新。 这个 filter 参数是可选的，主要是用来过滤你关心的模块，可以用来缩小快照的范围（注意：filter是过滤函数，不是字符串参数），为了让模块进入这个快照里，过滤函数的返回值要设置为true，反之设为false，如果后续内存中的模块加载信息更新了， 还会继续调用这个filter函数。
  - `has(address)` 检测地址是否处于模块中
  - `find(address), get(address)` 返回 address 地址所指向的模块对象详细信息，如果不存在 find 返回null，get 直接会抛出异常，具体的返回的对象的详细信息，可以参考 Process.enumerateModules()
  - `findName(address), getName(address), findPath(address), getPath(address)` 与find(address)和get(address)函数功能类似，但仅返回模块名称或者路径字段
  - `update()` 更新ModuleMap信息，如果有模块加载或者卸载，最好调用一次，免得使用旧数据
  - `values()` 返回map中的所有模块对象, 深拷贝(即再次调用update后值并不会发生改变)
### Memory
  - `Memory.scan(address, size, pattern, callbacks)` 从address内存开始，大小为size指定的内存中查找符合pattern模式的值，pattern形式"13 37 ?? ff" 或者更高级的掩码方式如"13 37 13 37 : 1f ff ff f1"其匹配的值为掩码的值&匹配的值，或者指定半字节的通配符"?3 37 13 ?7"
    - `onMatch(address, size)` 当匹配到对应的值NativePointer and Number
    - `onError(reason)` 当发生内存访问出现异常时会停止，reason会返回错误 String
    - `onComplete()` 当指定扫描内存扫描完毕时
  - `Memory.scanSync(address, size, pattern)` 同步扫描方式an array of objects
    - address: 绝对地址 NativePointer.
    - size: 大小
  ```javascript
  // Find the module for the program itself, always at index 0:
  const m = Process.enumerateModules()[0];

  // Or load a module by name:
  //const m = Module.load('win32u.dll');

  // Print its properties:
  console.log(JSON.stringify(m));

  // Dump it from its base address:
  console.log(hexdump(m.base));

  // The pattern that you are interested in:
  const pattern = '00 00 00 00 ?? 13 37 ?? 42';

  Memory.scan(m.base, m.size, pattern, {
    onMatch(address, size) {
      console.log('Memory.scan() found match at', address,
        'with size', size);
      // Optionally stop scanning early:
      return 'stop';
    },
    onComplete() {
      console.log('Memory.scan() complete');
    }
  });

  const results = Memory.scanSync(m.base, m.size, pattern);
  console.log('Memory.scanSync() result:\n' + JSON.stringify(results));
  ```
  - Memory.alloc(size[, options]): 在堆空间上申请指定大小的内存, 如果申请的大小为进程页面的大小，如果需要分配内存的地址接近给定的地址，可通过 { near: address, maxDistance: distanceInBytes }选项来获取接近指定地址的内存
  - Memory.copy(dst, src, n): 与c语言memcpy类似
  - Memory.dup(address, size): 与c语言dup类似, 先申请内存，在进行拷贝
  - Memory.protect(address, size, protection): 更新内存页面的属性 boolean
```javascript
  Memory.protect(ptr('0x1234'), 4096, 'rw-');
```
  - `Memory.patchCode(address, size, apply)` 安全打补丁，即修改指定address出内存中的值
```javascript
  const getLivesLeft = Module.getExportByName('game-engine.so', 'get_lives_left');
  const maxPatchSize = 64; // Do not write out of bounds, may be a temporary buffer!
  Memory.patchCode(getLivesLeft, maxPatchSize, code => {
    const cw = new X86Writer(code, {
      pc: getLivesLeft
    });
    cw.putMovRegU32('eax', 9000);
    cw.putRet();
    cw.flush();
  });
```
  - `Memory.allocUtf8String(str)`
  - `Memory.allocUtf16String(str)`
  - `Memory.allocAnsiString(str)`
### MemoryAccessMonitor
  - `MemoryAccessMonitor.enable(ranges, callbacks)` 监控一个或多个内存块的访问，在触发到内存访问的时候发出通知。ranges要么是一个单独的内存块，要么是一个内存块数组，每个内存块包含如下属性：
      - `ranges.base` 监视内存基地址
      - `ranges.size` 监视内存的大小
    callbacks对象的结构
      - `onAccess: function(details)` 发生访问的时候同步调用这个函数
        - `details.operation` 触发内存访问的操作类型，取值范围是(read, write, or execute)
        - `details.from` 触发内存访问的指令地址
        - `details.address` 被访问的内存地址
        - `details.rangeIndex` 被访问的内存块的索引，就是调用MemoryAccessMonitor.enable()的时候指定的内存块序号
        - `details.pageIndex` 在被监控内存块范围内的页面序号
        - `details.pagesCompleted` 到目前为止已经发生过内存访问的页面的个数（已经发生过内存访问的页面将不再进行监控）
        - `details.pagesTotal` 初始指定的需要监控的内存页面总数
  - `MemoryAccessMonitor.disable()` 停止监控页面访问操作
### CModule
new CModule(code[, symbols, options]): 根据提供的创建一个新的C模块, 该模块可以是包含带编译的C源代码的字符串， 也可以是预编译的共享库的ArrayBuffer，C模块被映射到内存中，并且可以被js代码完全访问，全局函数将会被自动导出为NativePointer todo
EXAMPLES
```javascript
  const cm = new CModule(`
    #include <stdio.h>
    void hello(void) {
      printf("Hello World from CModule\\n");
    }
`);
  console.log(JSON.stringify(cm));
  const hello = new NativeFunction(cm.hello, 'void', []);
  hello();
```
### ApiResolver
new ApiResolver(type): 创建给定类型的新解析器todo，解析器的类型有
  - module: Resolves exported and imported functions of shared libraries currently loaded. Always available.
  - objc: Resolves Objective-C methods of classes currently loaded. Available on macOS and iOS in processes that have the Objective-C runtime loaded. Use ObjC.available to check at runtime, or wrap your new ApiResolver('objc') call in a try-catch.
The resolver will load the minimum amount of data required on creation, and lazy-load the rest depending on the queries it receives. It is thus recommended to use the same instance for a batch of queries, but recreate it for future batches to avoid looking at stale data.

enumerateMatches(query): performs the resolver-specific query String, optionally suffixed with /i to perform case-insensitive matching, returning an array of objects containing the following properties:

name: name of the API that was found
address: address as a NativePointer
const resolver = new ApiResolver('module');
const matches = resolver.enumerateMatches('exports:*!open*');
const first = matches[0];
/*
 * Where `first` is an object similar to:
 *
 * {
 *   name: '/usr/lib/libSystem.B.dylib!opendir$INODE64',
 *   address: ptr('0x7fff870135c9')
 * }
 */
const resolver = new ApiResolver('objc');
const matches = resolver.enumerateMatches('-[NSURL* *HTTP*]');
const first = matches[0];
/*
 * Where `first` contains an object like this one:
 *
 * {
 *   name: '-[NSURLRequest valueForHTTPHeaderField:]',
 *   address: ptr('0x7fff94183e22')
 * }
 */

### DebugSymbol
  - `DebugSymbol.fromAddress(address), DebugSymbol.fromName(name)` 在指定地址或者指定名称查找符号信息，返回的符号信息对象包含下面的属性：
    - address: 当前符号的地址.
    - name: 当前符号的名称, 未知返回null
    - moduleName: 符号所在的模块名称, 未知返回null
    - fileName:  符号所在的文件名, 未知返回null
    - lineNumber: 符号所在的文件内的行号, 未知返回null
```
  const f = Module.getExportByName('libcommonCrypto.dylib',
      'CCCryptorCreate');
  Interceptor.attach(f, {
    onEnter(args) {
      //javascript Array map方法返回一个新数组，数组中的元素为原始数组元素调用函数处理后的值。
      //javascript Array join方法 把数组的所有元素放入一个字符串。
      console.log('CCCryptorCreate called from:\n' +
        Thread.backtrace(this.context, Backtracer.ACCURATE)
        .map(DebugSymbol.fromAddress).join('\n') + '\n');
    }
  });
```
  - `DebugSymbol.getFunctionByName(name)` 根据符号信息来查找函数，结果返回 NativePointer 对象
  - `DebugSymbol.findFunctionsNamed(name)` 根据符号信息来查找函数，结果返回 NativePointer 对象
  - `DebugSymbol.findFunctionsMatching(glob)` 根据符号信息来查找函数，结果返回 NativePointer 对象
  - `DebugSymbol.load(path)` 加载指定模块的符号信息
### Kernel（cy）
Kernel.available：一个布尔值，指定内核API是否可用。除非是这种情况，否则不要调用任何其他内核属性或方法。

Kernel.base: 内核的基址，如UInt64。

Kernel.pageSize: 内核页的大小，以字节为单位，以数字表示

Kernel.enumerateModules(): 枚举当前加载的内核模块，返回包含以下属性的对象数组：

?	name:作为字符串的规范模块名
?	base: 基本地址作为本地指针
?	size: 大小（字节）

Kernel.enumerateRanges(protection|specifier): 枚举满足保护要求的内核内存范围，格式为：rwx，其中rw-表示“必须至少可读写”。或者，您可以为说明符对象提供一个值如上所述的保护键，如果希望合并具有相同保护的相邻范围，则可以将合并键设置为true（默认值为false；i、 e.保持范围分开）。返回包含以下属性的对象数组：

?	base: 作为本机指针的基址
?	size: 大小（字节）
?	protection: 保护串（见上文）

Kernel.enumerateModuleRanges(name, protection):就像Kernel.enumerateRanges一样，只不过它的作用域是指定的模块名——对于内核本身的模块，该名称可能为空。每个范围还有一个名称字段，其中包含一个作为字符串的唯一标识符。

Kernel.alloc(size): 分配内核内存的大小字节，四舍五入到内核页面大小的倍数。返回值是一个UInt64，指定分配的基址

Kernel.protect(address, size, protection): 更新内核内存区域的保护，其中保护是与kernel.enumerateRanges（）格式相同的字符串。

举例:

Kernel.protect(UInt64('0x1234'), 4096, 'rw-');
	Kernel.readByteArray(address, length): just like NativePointer#readByteArray, but reading from kernel memory.

?	Kernel.writeByteArray(address, bytes): just like NativePointer#writeByteArray, but writing to kernel memory.

?	Kernel.scan(address, size, pattern, callbacks): just like Memory.scan, but scanning kernel memory.

?	Kernel.scanSync(address, size, pattern): synchronous version of scan() that returns the matches in an array.

## Data Types, Function and Callback

### Int64
- `new Int64(v)`: 以v为参数，创建一个Int64对象，v可以是一个数值，也可以是一个字符串形式的数值表示，也可以使用 Int64(v) 这种简单的方式。
- `add(rhs)`, `sub(rhs)`, `and(rhs)`, `or(rhs)`, `xor(rhs)` Int64相关的加减乘除或异或操作。
- `shr(n)`, `shl(n)` Int64相关的左移、右移操作
- `compare(rhs)` Int64的比较操作，有点类似String.localCompare()
- `toNumber()` 把Int64转换成一个实数
- `toString([radix = 10])` 按照一定的数值进制把Int64转成字符串，默认是十进制

### UInt64
- `new UInt64(v)`
- `add(rhs)`, `sub(rhs)`, `and(rhs)`, `or(rhs)`, `xor(rhs)`
- `shr(n)`, `shl(n)`
- `compare(rhs)`
- `toNumber()`
- `toString([radix = 10])`

### NativePointer
- `new NativePointer(s)`: 以字符串s为参数，创建一个NativePointer对象，也可以使用 ptr(s) 这种简单的方式。
- `isNull()` 返回一个布尔值，允许您方便地检查指针是否为空
- `add(rhs)`, `sub(rhs)`, `and(rhs)`, `or(rhs)`, `xor(rhs)`, `xor`rhs`
- `shr(n)`, `shl(n)`
- `not()`
- `sign([key, data])` todo
- `strip([key])` todo
- `blend(smallInteger)` todo
- `equals(rhs)`:返回一个布尔值，指示“rhs”是否等于此值；i、 它有相同的指针值
- `compare(rhs)`: 返回整数比较结果 just like **String#localeCompare()**
- `toInt32()`: casts this **NativePointer** to a signed 32-bit integer
- `toString([radix = 16])`:转换为可选基数的字符串（默认为16）
- `toMatchPattern()`: 返回包含[`Memory.scan（）`]的字符串(https://frida.re/docs/javascript-api/#memory-此指针的原始值的兼容匹配模式
- `readPointer()`:读取[`NativePointer`](https://frida.re/docs/javascript-api/#nativepointer)从这个记忆位置。
  如果地址不可读，将引发JavaScript异常。
- `writePointer(ptr)`: 将“ptr”写入此内存位置。
  如果地址不可写，将引发JavaScript异常。
- `readS8()`, `readU8()`, `readS16()`, `readU16()`, `readS32()`, `readU32()`, `readShort()`, `readUShort()`, `readInt()`, `readUInt()`, `readFloat()`, `readDouble()`: 从该内存位置读取有符号或无符号8/16/32/etc.或float/double值，并将其作为数字返回。
  如果地址不可读，将引发JavaScript异常。

- `writeS8(value)`, `writeU8(value)`, `writeS16(value)`, `writeU16(value)`, `writeS32(value)`, `writeU32(value)`, `writeShort(value)`, `writeUShort(value)`, `writeInt(value)`, `writeUInt(value)`, `writeFloat(value)`, `writeDouble(value)`: 将有符号或无符号8/16/32/etc.或float/double“value”写入此内存位置。
  如果地址不可写，将引发JavaScript异常。

- `readS64()`, `readU64()`, `readLong()`, `readULong()`:从该内存位置读取有符号或无符号的64位（或长大小）值，并将其作为**Int64**/**UInt64**值返回。
  如果地址不可读，将引发JavaScript异常。

- `writeS64(value)`, `writeU64(value)`, `writeLong(value)`, `writeULong(value)`: 将**Int64**/**UInt64**`value`写入此内存位置。
  如果地址不可写，将引发JavaScript异常。

- `readByteArray(length)`: re从该内存位置读取`length`字节，并将其作为**ArrayBuffer**返回。通过将缓冲区作为第二个参数传递到[`send（）`]可以有效地传输到基于Frida的应用程序(https://frida.re/docs/javascript-api/#communication-发送）。
  如果从地址读取的任何“长度”字节不可读，将引发JavaScript异常。

- `writeByteArray(bytes)`: 将“bytes”写入此内存位置，其中“bytes”是一个**ArrayBuffer**，通常从“readByteArray（）`”返回，或者是一个0到255之间的整数数组。`

   `. For example: `[ 0x13, 0x37, 0x42 ]`.

  如果写入地址的任何字节都不可写，则会引发JavaScript异常。

- `readCString([size = -1])`, `readUtf8String([size = -1])`, `readUtf16String([length = -1])`, `readAnsiString([size = -1])`: 以ASCII、UTF-8、UTF-16或ANSI字符串的形式读取此内存位置的字节。如果知道字符串的大小（以字节为单位），请提供可选的“size”参数；如果字符串以NUL结尾，请忽略该参数或指定*-1*。同样，如果您知道字符串的长度（以字符为单位），则可以提供可选的'length'参数。
  如果从地址读取的任何“size”/“length”字节不可读，将引发JavaScript异常。

  请注意，`readAnsiString（）`仅在Windows上可用（且相关）。

  `writeUtf8String(str)`, `writeUtf16String(str)`, `writeAnsiString(str)`:-将JavaScript字符串编码并写入此内存位置（使用NUL终止符）。
  如果写入地址的任何字节都不可写，则会引发JavaScript异常。
  注意，`WriteAsisting（）`仅在Windows上可用（和相关）。


### ArrayBuffer

- `wrap(address, size)`: 创建由现有内存区域支持的ArrayBuffer，其中address是指定该区域基址的NativePointer，size是指定其大小的数字。与NativePointer读/写api不同，access不执行任何验证，这意味着坏指针将使进程崩溃。
- `unwrap()`: 返回一个NativePointer，指定ArrayBuffer的备份存储的基址。调用方有责任在后备存储仍在使用时保持缓冲区的活动状态。

- `new NativeFunction(address, returnType, argTypes[, abi])`:创建一个新的NativeFunction来调用“address”处的函数（用[`NativePointer`]指定）(https://frida.re/docs/javascript-api/#nativepointer))，其中“returnType”指定返回类型，“argTypes”数组指定参数类型。如果不是系统默认值，也可以选择指定“abi”。对于可变函数，在固定参数和可变参数之间的“argTypes”中添加一个“…”条目。

### NativeFunction
- `new NativeFunction(address, returnType, argTypes[, abi])`: 在address（使用NativePointer的格式）地址上创建一个NativeFunction对象来进行函数调用，returnType指定函数返回类型，argTypes指定函数的参数类型，如果不是系统默认类型，也可以选择性的指定abi参数，对于可变类型的函数，在固定参数之后使用 “…” 来表示。
- 类和结构体
    在函数调用的过程中，类和结构体是按值传递的，传递的方式是使用一个数组来分别指定类和结构体的各个字段，理论上为了和需要的数组对应起来，这个数组是可以支持无限嵌套的，结构体和类构造完成之后，使用NativePointer的形式返回的，因此也可以传递给Interceptor.attach() 调用。需要注意的点是， 传递的数组一定要和需要的参数结构体严格吻合，比如一个函数的参数是一个3个整形的结构体，那参数传递的时候一定要是 [‘int’, ‘int’, ‘int’]，对于一个拥有虚函数的类来说，调用的时候，第一个参数一定是虚表指针。
- 支持类型
  - void
  - pointer
  - int
  - uint
  - long
  - ulong
  - char
  - uchar
  - size_t
  - ssize_t
  - float
  - double
  - int8
  - uint8
  - int16
  - uint16
  - int32
  - uint32
  - int64
  - uint64
  - bool
- 支持的调用约定
  - default
    - Windows 32-bit:
      - sysv
      - stdcall
      - thiscall
      - fastcall
      - mscdecl
    - Windows 64-bit:
      - win64
    - UNIX x86:
      - sysv
      - unix64
    - UNIX ARM:
      - sysv
      - vfp
`new NativeFunction(address, returnType, argTypes[, options])`:与前面的构造函数一样，但第四个参数options是一个对象，可能包含以下一个或多个键：
  - `option.abi`
  - `option.scheduling` cooperative: 允许其他线程在调用本机函数时执行JavaScript代码，即在调用前释放锁，然后重新获取。这是默认行为。exclusive: 在调用本机函数时不允许其他线程执行JavaScript代码，即保持JavaScript锁。这会更快，但可能会导致死锁。
  - `option.exceptions` steal: 如果被调用的函数产生异常（例如，通过取消对无效指针的引用），Frida将释放堆栈并获取异常，将其转换为可以处理的JavaScript异常。这可能会使应用程序处于未定义的状态，但有助于避免在试验时使进程崩溃。这是默认行为。propagate: 让应用程序处理函数调用期间发生的任何本机异常(或者，通过Process.setExceptionHandler（）安装的处理程序。）
  - `option.traps` default: 如果函数调用触发任何钩子，将调用Interceptor.attach（）回调。all: 除了拦截器回调，跟踪者也可以在每次函数调用期间临时重新激活。这对于测量代码覆盖率、指导fuzzer、在调试器中实现“单步执行”等非常有用。请注意，在使用Java和ObjC API时，这也是可能的，因为方法包装器还提供克隆（options）API来创建具有自定义NativeFunction选项的新方法包装器。

### NativeCallback
- `new NativeCallback(func, returnType, argTypes[, abi])`: 使用JavaScript函数 func 来创建一个Native函数，其中returnType和argTypes分别指定函数的返回类型和参数类型数组。如果不想使用系统默认的 abi 类型，则可以指定 abi 这个参数。关于argTypes和abi类型，可以查看NativeFunction来了解详细信息，这个对象的返回类型也是NativePointer类型，因此可以作为 Interceptor.replace 的参数使用。
### SystemFunction
- `new SystemFunction(address, returnType, argTypes[, abi])` 功能基本和NativeFunction一致，但是使用这个对象可以获取到调用线程的last error状态，返回值是对平台相关的数值的一层封装，为value对象，比如是对这两个值的封装， errno(UNIX) 或者 lastError(Windows)。
- `new SystemFunction(address, returnType, argTypes[, options])`: 同上，接受option对象，与NativeFunction构造函数类似。

## Network

### Socket

- `Socket.listen([options])`: 打开TCP或UNIX侦听套接字。返回接收SocketListener的句柄。
  默认为同时侦听IPv4和IPv6（如果支持），并绑定随机选择的TCP端口上的所有接口。
  可选选项参数是一个可能包含以下某些键的对象：
  - `family` 协议地址族(unix | ipv4 | ipv6)
  - `host`: 主机地址
  - `port`: 主机端口
  - `type`: unix协议族socket类型(anonymous | path | abstract | abstract-padded)
  - `path`: UNIX socket path as a string.
  - `backlog`: 监听队列数 number. Defaults to `10`.
- `Socket.connect(options)`: 连接TCP Server或者Unix Server. Returns a *Promise* that receives a **SocketConnection**.

  The `options` argument is an object that should contain some of the following keys:

  - ```
    family
    ```

    : address family as a string. Supported values are:

    - unix
    - ipv4
    - ipv6 Defaults to an IP family depending on the `host` specified.

  - `host`: (IP family) IP address as a string. Defaults to `localhost`.

  - `port`: (IP family) IP port as a number.

  - ```
    type
    ```

    : (UNIX family) UNIX socket type as a string. Supported types are:

    - anonymous
    - path
    - abstract
    - abstract-padded Defaults to `path`.

  - `path`: (UNIX family) UNIX socket path as a string.

- `Socket.type(handle)`: inspect the OS socket `handle` and return its type as a string which is either `tcp`, `udp`, `tcp6`, `udp6`, `unix:stream`, `unix:dgram`, or `null` if invalid or unknown.

- `Socket.localAddress(handle)`, `Socket.peerAddress(handle)`: inspect the OS socket `handle` and return its local or peer address, or `null` if invalid or unknown.

  The object returned has the fields:

  - `ip`: (IP sockets) IP address as a string.
  - `port`: (IP sockets) IP port as a number.
  - `path`: (UNIX sockets) UNIX path as a string.

### SocketListener

所有方法都是完全异步的，并返回Promise对象。

- `path`: (UNIX family) path being listened on.
- `port`: (IP family) IP port being listened on.
- `close()`:关闭侦听器，释放与其相关的资源。一旦侦听器关闭，所有其他操作都将失败。允许多次关闭侦听器，并且不会导致错误。
- `accept()`: 等待下一个客户端连接。返回的承诺接收SocketConnection。

### SocketConnection

继承IOStream。所有方法都是完全异步的，并返回Promise对象。



- `setNoDelay(noDelay)`: 如果noDelay为true，则禁用Nagle算法，否则启用它。Nagle算法在默认情况下是启用的，因此只有在希望优化低延迟而不是高吞吐量时才需要调用此方法。

------

## File and Stream
### File
- `new File(filePath, mode)` 在filePath处打开或创建文件，模式字符串指定如何打开该文件。例如“wb”以二进制模式打开文件进行写入(与C标准库中的fopen()格式相同)
- `write(data)`: 同步地将数据写入文件，其中的数据可以是字符串，也可以是由NativePointer#readByteArray返回的缓冲区
- `flush()`: f将所有缓冲数据刷新到基础文件
- `close()`: 关闭文件。您应该在处理完文件后调用此函数，除非您对对象被垃圾收集或脚本被卸载时发生的情况没有意见。
```javascript
var file = new File("/sdcard/reg.dat", "r+");
file.write("EoPAoY62@ElRD");
file.flush();
file.close();
```

### IOStream
所有方法都是完全异步的，并返回Promise对象。
- `input`: the **InputStream** to read from.
- `output`: the **OutputStream** to write to.
- `close()`: 关闭流，释放与之相关的资源。这也将关闭单个输入和输出流。一旦流关闭，所有其他操作都将失败。允许多次关闭流，不会导致错误。

### InputStream

所有方法都是完全异步的，并返回Promise对象。

- `close()`: 关闭流，释放与之相关的资源。一旦流关闭，所有其他操作都将失败。允许多次关闭流，并且不会导致错误。
- `read(size)`: 从流中读取到大小字节。返回的承诺接收一个长达字节的ArrayBuffer。流的末尾通过一个空缓冲区发出信号。
- `readAll(size)`: 继续从流中读取，直到消耗了确切的大小字节。返回的承诺接收一个ArrayBuffer，它的长度正好是字节。过早错误或流的结束会导致承诺被拒绝，并出现错误，其中错误对象具有包含不完整数据的partialData属性。

### OutputStream

所有方法都是完全异步的，并返回Promise对象。
	`close()`: 关闭流，释放与其相关的资源。一旦流关闭，所有其他操作都将失败。允许多次关闭流，并且不会导致错误。

- `write(data)`: 尝试将数据写入流。数据值可以是ArrayBuffer，也可以是0到255之间的整数数组。返回的Promise接收一个数字，指定向流中写入了多少字节的数据。
- `writeAll(data)`: 一直写入流，直到所有数据都被写入。数据值是ArrayBuffer或介于0和255之间的整数数组。过早错误或流结束将导致错误，其中错误对象具有partialSize属性，指定在错误发生之前向流写入了多少字节的数据。
- `writeMemoryRegion(address, size)`: 尝试将大小字节写入流，从地址（NativePointer）读取它们。返回的Promise接收一个数字，该数字指定写入流的数据字节数。

### UnixInputStream

(Only available on UNIX-like OSes.)



- `new UnixInputStream(fd[, options])`: 从指定的文件描述符fd创建新的InputStream。
  您还可以提供一个options对象，将autoClose设置为true，以便在释放流时，通过close（）或将来的垃圾回收，使流关闭底层文件描述符。

### UnixOutputStream

(Only available on UNIX-like OSes.)



- `new UnixOutputStream(fd[, options])`: 从指定的文件描述符fd创建新的OutputStream。
  您还可以提供一个options对象，将autoClose设置为true，以便在释放流时，通过close（）或将来的垃圾回收，使流关闭底层文件描述符。

### Win32InputStream

(Only available on Windows.)



- `new Win32InputStream(handle[, options])`: 从指定的句柄（Windows句柄值）创建新的InputStream。
  您还可以提供一个options对象，将autoClose设置为true，以便在释放流时，通过close（）或将来的垃圾回收，使流关闭底层句柄。

### Win32OutputStream

(Only available on Windows.)



- `new Win32OutputStream(handle[, options])`: 从指定的句柄（Windows句柄值）创建新的OutputStream。
  您还可以提供一个options对象，将autoClose设置为true，以便在释放流时，通过close（）或将来的垃圾回收，使流关闭底层句柄。


#### 数据库
### Sqlite数据库
- SqliteDatabase.open(path[, options]): 打开由 指定的 SQLite v3 数据库path，一个包含数据库文件系统路径的字符串。默认情况下，数据库将以读写方式打开，但您可以通过提供具有options名为 的属性的对象并flags指定包含以下一个或多个值的字符串数组来自定义此行为：readonly、readwrite、create。返回的 SqliteDatabase 对象将允许您对数据库执行查询。

- SqliteDatabase.openInline(encodedContents): 就像open()但数据库的内容作为包含其数据的字符串提供，Base64 编码。我们建议在对数据库进行 Base64 编码之前对数据库进行 gzip，但这是可选的，并且可以通过查找 gzip 魔术标记来检测。数据库以读写方式打开，但 100% 在内存中，从不接触文件系统。这对于需要捆绑预计算数据缓存的代理很有用，例如用于指导动态分析的静态分析数据。

- close(): 关闭数据库。完成数据库操作后，您应该调用此函数，除非在对象被垃圾收集或脚本卸载时发生这种情况。

- exec(sql)：执行原始 SQL 查询，其中sql是包含查询文本表示的字符串。查询的结果将被忽略，因此这应该仅用于设置数据库的查询，例如表创建。

- prepare(sql): 将提供的 SQL 编译成 SqliteStatement对象，其中sql是包含查询文本表示的字符串。
例如：
```javascript
const db = SqliteDatabase.open('/path/to/people.db');

const smt = db.prepare('SELECT name, bio FROM people WHERE age = ?');

console.log('People whose age is 42:');
smt.bindInteger(1, 42);
let row;
while ((row = smt.step()) !== null) {
  const [name, bio] = row;
  console.log('Name:', name);
  console.log('Bio:', bio);
}
smt.reset();
```

- dump(): 将数据库转储到编码为 Base64 的 gzip 压缩 blob，其中结果作为字符串返回。这对于在代理代码中内联缓存很有用，通过调用SqliteDatabase.openInline().


### Sqlite语句
- bindInteger(index, value): 将整数绑定value到index
- bindFloat(index, value): 将浮点绑定value到index
- bindText(index, value): 将文本绑定value到index
- bindBlob(index, bytes): 将 blob 绑定bytes到index，其中bytes 是ArrayBuffer、字节值数组或字符串
- bindNull(index): 将一个空值绑定到 index
- step(): 要么开始一个新的查询并获得第一个结果，要么移动到下一个。返回一个数组，其中包含按查询指定的顺序或null在达到最后一个结果时的值。reset()如果您打算再次使用此对象，则应在此时调用 。
- reset(): 重置内部状态以允许后续查询

#### 仪表
### 拦截器
- `Interceptor.attach(target, callbacks[, data])`:  在target指定的位置进行函数调用拦截，target是一个NativePointer参数，用来指定你想要拦截的函数的地址，有一点需要注意，在32位ARM机型上，ARM函数地址末位一定是0（2字节对齐），Thumb函数地址末位一定1（单字节对齐），如果使用的函数地址是用Frida API获取的话， 那么API内部会自动处理这个细节（比如：Module.findExportByName()）。事实上Frida可以在代码的任意位置进行拦截，但是这样一来 callbacks 回调的时候，因为回调位置有可能不在函数的开头，这样onEnter这样的回调参数Frida只能尽量的保证（比如拦截的位置前面的代码没有修改过传入的参数），不能像在函数头那样可以确保正确。拦截器的attach调用返回一个监听对象，后续取消拦截的时候，可以作为 Interceptor.detach() 的参数使用，还有一个比较方便的地方，那就是在回调函数里面，包含了一个隐藏的 this 的线程tls对象，方便在回调函数中存储变量，比如可以在 onEnter 中保存值，然后在 onLeave 中使用。此外，this对象中还包含 returnAddress: 返回NativePointer类型的 address 对象, context: 包含 pc，sp，以及相关寄存器比如 eax, ebx等，可以在回调函数中直接修改, errno: UNIX）当前线程的错误值. lastError: (Windows) 当前线程的错误值, threadId: 操作系统线程 ID, depth: 函数调用层次深度. 性能注意事项：提供的回调对性能有重大影响。如果您只需要检查参数而不关心返回值，或者只关心返回值不关系参数，请确保省略不需要的回调；即避免将您的逻辑放在onEnter中并将onLeave作为空回调留在那里。在iPhone 5S上，仅提供onEnter时的基本开销可能是 6 微秒，而在提供onEnter 和onLeave 时则为11 微秒。还要小心拦截对每秒调用无数次的函数的调用；虽然send()是异步的，但发送单个消息的总开销并未针对高频进行优化。但是，当挂钩热函数时，您可以结合Interceptor 与CModule的使用用于在 C 中实现回调。其中callbacks参数是一个对象，大致结构如下：
  - `onEnter: function(args)`: 被拦截函数调用之前回调，其中原始函数的参数使用args数组（NativePointer对象数组）来表示，可以在这里修改函数的调用参数。
  - `onLeave: function(retval)`：被拦截函数调用之后回调，其中retval表示原始函数的返回值，retval是从NativePointer继承来的，是对原始返回值的一个封装，你可以使用retval.replace(1337)调用来修改返回值的内容。需要注意的一点是，retval对象只在 onLeave函数作用域范围内有效，因此如果你要保存这个对象以备后续使用的话，一定要使用深拷贝来保存对象，比如：ptr(retval.toString())。
```javascript
Interceptor.attach(Module.getExportByName('libc.so', 'read'), {
  onEnter(args) {
    this.fileDescriptor = args[0].toInt32();
  },
  onLeave(retval) {
    if (retval.toInt32() > 0) {
      /* do something with this.fileDescriptor */
    }
  }
});
```

```javascript
Interceptor.attach(Module.getExportByName(null, 'read'), {
  onEnter(args) {
    console.log('Context information:');
    console.log('Context  : ' + JSON.stringify(this.context));
    console.log('Return   : ' + this.returnAddress);
    console.log('ThreadId : ' + this.threadId);
    console.log('Depth    : ' + this.depth);
    console.log('Errornr  : ' + this.err);

    // Save arguments for processing in onLeave.
    this.fd = args[0].toInt32();
    this.buf = args[1];
    this.count = args[2].toInt32();
  },
  onLeave(result) {
    console.log('----------')
    // Show argument 1 (buf), saved during onEnter.
    const numBytes = result.toInt32();
    if (numBytes > 0) {
      console.log(hexdump(this.buf, { length: numBytes, ansi: true }));
    }
    console.log('Result   : ' + numBytes);
  }
})
```
- `Interceptor.detachAll()`: 取消之前所有的拦截调用
- `Interceptor.replace(target, replacement[, data])`: 函数实现代码替换，这种情况主要是你想要完全替换掉一个原有函数的实现的时候来使用，注意replacement参数使用JavaScript形式的一个NativeCallback来实现，后续如果想要取消这个替换效果，可以使用 Interceptor.revert调用来实现，如果你还想在你自己的替换函数里面继续调用原始的函数，可以使用以 target 为参数的NativeFunction对象来调用
```javascript
const openPtr = Module.getExportByName('libc.so', 'open');
const open = new NativeFunction(openPtr, 'int', ['pointer', 'int']);
Interceptor.replace(openPtr, new NativeCallback((pathPtr, flags) => {
  const path = pathPtr.readUtf8String();
  log('Opening "' + path + '"');
  const fd = open(pathPtr, flags);
  log('Got fd: ' + fd);
  return fd;
}, 'int', ['pointer', 'int']));
```
- `Interceptor.revert(target)` 还原函数的原始实现逻辑，即取消前面的 Interceptor.replace调用
- `Interceptor.flush()` 确保之前的内存修改操作都执行完毕，并且已经在内存中发生作用，只要少数几种情况需要这个调用，比如你刚执行了 attach() 或者 replace() 调用，然后接着想要使用NativeFunction对象对函数进行调用，这种情况就需要调用flush。正常情况下，缓存的调用操作会在当前线程即将离开JavaScript运行时环境或者调用send()的时候自动进行flush操作，也包括那些底层会调用 send() 操作的函数，比如 RPC 函数，或者任何的console API

#### Stalker
- `Stalker.exclude(range)`: 将指定的内存标记range为排除的，它是一个具有base和size属性的对象——就像由 eg 返回的对象中的属性Process.getModuleByName()。

这意味着当遇到对此类范围内的指令的调用时，Stalker 将不会跟随执行。因此，您将能够观察/修改传入的参数和返回的返回值，但不会看到发生在两者之间的指令。

有助于提高性能和降低噪音。

- `Stalker.follow([threadId, options])` 开始监视线程ID为 threadId（如果是本线程，可以省略）的线程事件
```javascript
const mainThread = Process.enumerateThreads()[0];

Stalker.follow(mainThread.id, {
  events: {
    call: true, // CALL instructions: yes please

    // Other events:
    ret: false, // RET instructions
    exec: false, // all instructions: not recommended as it's
                 //                   a lot of data
    block: false, // block executed: coarse execution trace
    compile: false // block compiled: useful for coverage
  },

  //
  // Only specify one of the two following callbacks.
  // (See note below.)
  //

  //
  // onReceive: Called with `events` containing a binary blob
  //            comprised of one or more GumEvent structs.
  //            See `gumevent.h` for details about the
  //            format. Use `Stalker.parse()` to examine the
  //            data.
  //
  //onReceive(events) {
  //},
  //

  //
  // onCallSummary: Called with `summary` being a key-value
  //                mapping of call target to number of
  //                calls, in the current time window. You
  //                would typically implement this instead of
  //                `onReceive()` for efficiency, i.e. when
  //                you only want to know which targets were
  //                called and how many times, but don't care
  //                about the order that the calls happened
  //                in.
  //
  onCallSummary(summary) {
  },

  //
  // Advanced users: This is how you can plug in your own
  //                 StalkerTransformer, where the provided
  //                 function is called synchronously
  //                 whenever Stalker wants to recompile
  //                 a basic block of the code that's about
  //                 to be executed by the stalked thread.
  //
  //transform(iterator) {
  //  let instruction = iterator.next();
  //
  //  const startAddress = instruction.address;
  //  const isAppCode = startAddress.compare(appStart) >= 0 &&
  //      startAddress.compare(appEnd) === -1;
  //
  //  do {
  //    if (isAppCode && instruction.mnemonic === 'ret') {
  //      iterator.putCmpRegI32('eax', 60);
  //      iterator.putJccShortLabel('jb', 'nope', 'no-hint');
  //
  //      iterator.putCmpRegI32('eax', 90);
  //      iterator.putJccShortLabel('ja', 'nope', 'no-hint');
  //
  //      iterator.putCallout(onMatch);
  //
  //      iterator.putLabel('nope');
  //    }
  //
  //    iterator.keep();
  //  } while ((instruction = iterator.next()) !== null);
  //},
  //
  // The default implementation is just:
  //
  //   while (iterator.next() !== null)
  //     iterator.keep();
  //
  // The example above shows how you can insert your own code
  // just before every `ret` instruction across any code
  // executed by the stalked thread inside the app's own
  // memory range. It inserts code that checks if the `eax`
  // register contains a value between 60 and 90, and inserts
  // a synchronous callout back into JavaScript whenever that
  // is the case. The callback receives a single argument
  // that gives it access to the CPU registers, and it is
  // also able to modify them.
  //
  // function onMatch (context) {
  //   console.log('Match! pc=' + context.pc +
  //       ' rax=' + context.rax.toInt32());
  // }
  //
  // Note that not calling keep() will result in the
  // instruction getting dropped, which makes it possible
  // for your transform to fully replace certain instructions
  // when this is desirable.
  //

  //
  // Want better performance? Write the callbacks in C:
  //
  // /*
  //  * const cm = new CModule(\`
  //  *
  //  * #include <gum/gumstalker.h>
  //  *
  //  * static void on_ret (GumCpuContext * cpu_context,
  //  *     gpointer user_data);
  //  *
  //  * void
  //  * transform (GumStalkerIterator * iterator,
  //  *            GumStalkerOutput * output,
  //  *            gpointer user_data)
  //  * {
  //  *   cs_insn * insn;
  //  *
  //  *   while (gum_stalker_iterator_next (iterator, &insn))
  //  *   {
  //  *     if (insn->id == X86_INS_RET)
  //  *     {
  //  *       gum_x86_writer_put_nop (output->writer.x86);
  //  *       gum_stalker_iterator_put_callout (iterator,
  //  *           on_ret, NULL, NULL);
  //  *     }
  //  *
  //  *     gum_stalker_iterator_keep (iterator);
  //  *   }
  //  * }
  //  *
  //  * static void
  //  * on_ret (GumCpuContext * cpu_context,
  //  *         gpointer user_data)
  //  * {
  //  *   printf ("on_ret!\n");
  //  * }
  //  *
  //  * void
  //  * process (const GumEvent * event,
  //  *          GumCpuContext * cpu_context,
  //  *          gpointer user_data)
  //  * {
  //  *   switch (event->type)
  //  *   {
  //  *     case GUM_CALL:
  //  *       break;
  //  *     case GUM_RET:
  //  *       break;
  //  *     case GUM_EXEC:
  //  *       break;
  //  *     case GUM_BLOCK:
  //  *       break;
  //  *     case GUM_COMPILE:
  //  *       break;
  //  *     default:
  //  *       break;
  //  *   }
  //  * }
  //  * `);
  //  */
  //
  //transform: cm.transform,
  //onEvent: cm.process,
  //data: ptr(1337) /* user_data */
  //
  // You may also use a hybrid approach and only write
  // some of the callouts in C.
  //
});
```

性能注意事项
提供的回调对性能有重大影响。如果您只需要定期调用摘要而不关心原始事件，或者相反，请确保省略不需要的回调；即避免将您的逻辑放在onCallSummary 中并将 onReceive作为空回调留在那里。

另请注意，Stalker 可以与CModule结合使用，这意味着回调可以在 C 中实现。

- `Stalker.unfollow([threadId])` 停止监控线程事件，如果是当前线程，则可以省略 threadId 参数
- `Stalker.parse(events[, options])` todo
```javascript
  onReceive(events) {
    console.log(Stalker.parse(events, {
      annotate: true, // to display the type of event
      stringify: true
        // to format pointer values as strings instead of `NativePointer`
        // values, i.e. less overhead if you're just going to `send()` the
        // thing not actually parse the data agent-side
    }));
  },
```  
- `Stalker.flush()`: 清除任何缓冲的事件。当您不想等到下一个Stalker.queueDrainInterval滴答声时很有用。
- `Stalker.garbageCollect()`: 在调用Stalker.unfollow()之后，在一个合适的时候，释放对应的内存，可以避免多线程竞态条件下的内存释放问题。
- `Stalker.invalidate(address)`: 使给定基本块的当前线程的翻译代码无效。在提供转换回调并希望为给定的基本块动态调整检测时很有用。这比取消关注和重新关注线程要高效得多，后者会丢弃所有缓存的翻译并要求从头开始编译所有遇到的基本块。

- `Stalker.invalidate(threadId, address)`: 使给定基本块的特定线程的翻译代码无效。在提供转换回调并希望为给定的基本块动态调整检测时很有用。这比取消关注和重新关注线程要高效得多，后者会丢弃所有缓存的翻译并要求从头开始编译所有遇到的基本块。

- `Stalker.addCallProbe(address, callback[, data])` 当address地址处的函数被调用的时候，调用callback对象（对象类型和Interceptor.attach.onEnter一致），返回一个Id，可以给后面的Stalker.removeCallProbe使用
  - `void onCall (GumCallSite * site, gpointer user_data)`
在这种情况下，第三个可选参数data可能是 a，NativePointer 其值作为 传递给回调user_data。

- `Stalker.removeCallProbe`: 删除由 Stalker#addCallProbe.

- `Stalker.trustThreshold`: 一个整数，指定一段代码需要执行多少次才能被认为可以信任它不会发生变异。指定 -1 表示不信任（慢），0 表示从一开始就信任代码，N 表示在执行 N 次后信任代码。默认为 1。

- `Stalker.queueCapacity` 指定事件队列的长度，默认长度是16384
- `Stalker.queueDrainInterval` 事件队列查询派发时间间隔，默认是250ms，也就是说1秒钟事件队列会轮询4次

#### 弱引用
- `WeakRef.bind(value, fn)`：一旦被垃圾收集或脚本即将被卸载，就监视value并调用fn回调value。返回一个可以传递给WeakRef.unbind() 显式清理的 id 。

如果您正在构建语言绑定，当不再需要 JS 值时需要释放本机资源，则此 API 很有用。

- `WeakRef.unbind(id)`: 停止监视传递给 的值 WeakRef.bind(value, fn)，并fn立即调用回调。

#### Java
- `Java.available` 一个布尔值，表示当前进程中是否存在完整可用的Java虚拟机环境，Dalvik或者Art，建议在使用Java方法之前，使用这个变量来确保环境正常。
- `Java.androidVersion` 一个字符串，指定我们正在运行的 Android 版本。
- `Java.enumerateLoadedClasses(callbacks)` 枚举当前进程中已经加载的类，每次枚举到加载的类回调callbacks:
  - `onMatch(name, handle)` 枚举到一个类，以类名称进行回调，这个类名称后续可以作为 Java.use() 的参数来获取该类的一个引用对象。
  - `onComplete()`: 在枚举完所有类时调用。
- `Java.enumerateLoadedClassesSync()`: enumerateLoadedClasses()同步版本 
- `Java.enumerateClassLoaders(callbacks)`: 枚举 Java VM 中存在的类加载器，其中callbacks是指定的对象：
  - `onMatch(loader)`: 为每个类加载器调用，loader特定的包装器java.lang.ClassLoader。
  - `onComplete()`: 在枚举所有类加载器时调用。
您可以将这样的加载器传递Java.ClassFactory.get()给 .use()指定的类加载器上的类。
- `Java.enumerateClassLoadersSync()`: 同步版本 enumerateClassLoaders()返回数组中的类加载器。
- `Java.enumerateMethods(query)`: 枚举匹配的方法query，指定为"class!method"，允许使用 globs。也可以后缀/和一个或多个修饰符：
  - `i`: 不区分大小写的匹配。
  - `s`: 包括方法签名，所以 eg"putInt"变成 "putInt(java.lang. String, int): void".
  - `u`: 仅用户定义的类，忽略系统类。
```javascript
Java.perform(() => {
  const groups = Java.enumerateMethods('*youtube*!on*')
  console.log(JSON.stringify(groups, null, 2));
});
```
```
[
  {
    "loader": "<instance: java.lang.ClassLoader, $className: dalvik.system.PathClassLoader>",
    "classes": [
      {
        "name": "com.google.android.apps.youtube.app.watch.nextgenwatch.ui.NextGenWatchLayout",
        "methods": [
          "onAttachedToWindow",
          "onDetachedFromWindow",
          "onFinishInflate",
          "onInterceptTouchEvent",
          "onLayout",
          "onMeasure",
          "onSizeChanged",
          "onTouchEvent",
          "onViewRemoved"
        ]
      },
      {
        "name": "com.google.android.apps.youtube.app.search.suggest.YouTubeSuggestionProvider",
        "methods": [
          "onCreate"
        ]
      },
      {
        "name": "com.google.android.libraries.youtube.common.ui.YouTubeButton",
        "methods": [
          "onInitializeAccessibilityNodeInfo"
        ]
      },
      …
    ]
  }
]
```
- `Java.scheduleOnMainThread(fn)` 在虚拟机主线程上执行函数fn
- `Java.perform(fn)`: 确保当前线程附加到 VM 并调用fn. （这在来自 Java 的回调中不是必需的。）fn如果应用程序的类加载器尚不可用，则将推迟调用。Java.performNow()如果不需要访问应用程序的类，请使用。
```javascript
Java.perform(() => {
  const Activity = Java.use('android.app.Activity');
  Activity.onResume.implementation = function () {
    send('onResume() got called! Let\'s call the original implementation');
    this.onResume();
  };
});
```
- `Java.performNow(fn)`: 确保当前线程附加到 VM 并调用fn. （这在来自 Java 的回调中不是必需的。）
- `Java.use(className)`: 动态获取一个 JavaScript 包装器 className，您可以通过调用$new()它来调用构造函数来实例化对象。调用$dispose()一个实例来显式地清理它（或等待 JavaScript 对象被垃圾收集，或脚本被卸载）。静态和非静态方法都可用，您甚至可以替换方法实现并从中抛出异常：
```javascript
Java.perform(() => {
  const Activity = Java.use('android.app.Activity');
  const Exception = Java.use('java.lang.Exception');
  Activity.onResume.implementation = function () {
    throw Exception.$new('Oh noes!');
  };
});
```
默认情况下使用应用程序的类加载器，但您可以通过将不同的加载器实例分配给Java.classFactory.loader.

请注意，所有方法包装器都提供了一个clone(options)API 来创建具有自 定义NativeFunction选项的新方法包装器。

- `Java.openClassFile(filePath)`: 打开 .dex 文件filePath，使用以下方法返回一个对象：

  - `oad()`: 将包含的类加载到 VM 中。
  
  - `etClassNames()`: 获取可用类名的数组。

- `Java.choose(className, callbacks)` 在Java的内存堆上扫描指定类名称的Java对象，每次扫描到一个对象，则回调callbacks:
  - `onMatch(instance)` 每次扫描到一个实例对象，调用一次，函数返回stop结束扫描的过程
  - `onComplete()` 当所有的对象都扫描完毕之后进行回调
- `Java.retain(obj)`: 复制 JavaScript 包装器obj供以后在替换方法之外使用。
```javascript
Java.perform(() => {
  const Activity = Java.use('android.app.Activity');
  let lastActivity = null;
  Activity.onResume.implementation = function () {
    lastActivity = Java.retain(this);
    this.onResume();
  };
});
```

- `Java.cast(handle, klass)` 使用对象句柄handle按照klass（Java.use方法返回）的类型创建一个对象的JavaScript引用，这个对象引用包含一个class属性来获取当前对象的类，也包含一个$className属性来获取类名称字符串，比如：
```javascript
const Activity = Java.use('android.app.Activity');
const activity = Java.cast(ptr('0x1234'), Activity);
```

- `Java.array(type, elements)`:type从 JavaScript 数组创建具有指定元素的Java 数组elements。生成的 Java 数组的行为类似于 JS 数组，但可以通过引用传递给 Java API 以允许它们修改其内容。
```java
const values = Java.array('int', [ 1003, 1005, 1007 ]);

const JString = Java.use('java.lang.String');
const str = JString.$new(Java.array('byte', [ 0x48, 0x65, 0x69 ]));
```

- `Java.isMainThread()`: 判断调用者是否在主线程上运行。

- `Java.registerClass(spec)`：创建一个新的 Java 类并为其返回一个包装器，其中spec是一个包含以下内容的对象：

  - `name`: 指定类名的字符串。
  - `superClass`：（可选）超级。省略继承自 java.lang.Object.
  - `implements`：（可选）此类实现的接口数组。
  - `fields`：（可选）指定要公开的每个字段的名称和类型的对象。
  - `methods`：（可选）对象指定要实现的方法。

```java  
const SomeBaseClass = Java.use('com.example.SomeBaseClass');
const X509TrustManager = Java.use('javax.net.ssl.X509TrustManager');

const MyTrustManager = Java.registerClass({
  name: 'com.example.MyTrustManager',
  implements: [X509TrustManager],
  methods: {
    checkClientTrusted(chain, authType) {
    },
    checkServerTrusted(chain, authType) {
    },
    getAcceptedIssuers() {
      return [];
    },
  }
});

const MyWeirdTrustManager = Java.registerClass({
  name: 'com.example.MyWeirdTrustManager',
  superClass: SomeBaseClass,
  implements: [X509TrustManager],
  fields: {
    description: 'java.lang.String',
    limit: 'int',
  },
  methods: {
    $init() {
      console.log('Constructor called');
    },
    checkClientTrusted(chain, authType) {
      console.log('checkClientTrusted');
    },
    checkServerTrusted: [{
      returnType: 'void',
      argumentTypes: ['[Ljava.security.cert.X509Certificate;', 'java.lang.String'],
      implementation(chain, authType) {
        console.log('checkServerTrusted A');
      }
    }, {
      returnType: 'java.util.List',
      argumentTypes: ['[Ljava.security.cert.X509Certificate;', 'java.lang.String', 'java.lang.String'],
      implementation(chain, authType, host) {
        console.log('checkServerTrusted B');
        return null;
      }
    }],
    getAcceptedIssuers() {
      console.log('getAcceptedIssuers');
      return [];
    },
  }
});
```

- `Java.deoptimizeEverything()`: 强制 VM 使用其解释器执行所有操作。在某些情况下有必要防止优化绕过方法挂钩，并允许使用 ART 的 Instrumentation API 来跟踪运行时。
- `Java.deoptimizeBootImage()`: 类似于 Java.deoptimizeEverything() 但只对启动映像代码进行去优化。使用以 dalvik.vm.dex2oat-flags --inline-max-code-units=0获得最佳效果。
- `Java.vm`：具有以下方法的对象：
  - `perform(fn)`: 确保当前线程附加到 VM 并调用fn. （这在来自 Java 的回调中不是必需的。）
  - `getEnv()`: 获取当前线程的JNIEnv. 如果当前线程未附加到 VM，则引发异常。
  - `tryGetEnv()`: 尝试为当前线程的JNIEnv. 返回null如果当前线程没有连接到虚拟机。
- `Java.classFactory`：用于实现例如的默认类工厂 Java.use()。使用应用程序的主类加载器。
- `Java.ClassFactory`: 具有以下属性的类：
  - `get(classLoader)`: 获取给定类加载器的类工厂实例。幕后使用的默认类工厂仅与应用程序的主类加载器交互。可以Java.enumerateClassLoaders()通过此 API发现其他类加载器并与之交互。
  - `loader`: 只读属性，为当前使用的类加载器提供包装器。对于默认的类工厂，这通过第一次调用来更新Java.perform()。
  - `cacheDir`: 包含当前使用的缓存目录路径的字符串。对于默认的类工厂，这通过第一次调用来更新Java.perform()。
  - `tempFileNaming`: 指定用于临时文件的命名约定的对象。默认为{ prefix: 'frida', suffix: 'dat' }.
  - `use(className)`: 就像Java.use()但对于特定的类加载器。
  - `openClassFile(filePath)`: 就像Java.openClassFile() 但对于特定的类加载器。
  - `choose(className, callbacks)`: 就像Java.choose()但对于特定的类加载器。
  - `retain(obj)`: 就像Java.retain()但对于特定的类加载器。
  - `cast(handle, klass)`: 就像Java.cast()但对于特定的类加载器。
  - `array(type, elements)`: 就像Java.array()但对于特定的类加载器。
  - `registerClass(spec)`: 就像Java.registerClass()但对于特定的类加载器。
### CPU指令
- `Instruction.parse(target)` 在 target 指定的地址处解析指令，其中target是一个NativePointer。注意，在32位ARM上，ARM函数地址需要是2字节对齐的，Thumb函数地址是1字节对齐的，如果你是使用Frida本身的函数来获取的target地址，Frida会自动处理掉这个细节，关于operands和groups的细节，请参考CapStone文档. parse函数返回的对象包含如下属性
  - address：当前指令的EIP，NativePointer类型
  - next：下条指令的地址，可以继续使用parse函数
  - size: 当前指令大小
  - mnemonic: 指令助记符
  - opStr: 字符串格式显示操作数
  - operands：操作数数组，每个操作数对象包含type和value两个属性，根据平台不同，有可能还包含一些额外属性
  - regsRead: 这条指令显式进行读取的寄存器数组
  - regsWritten: 这条指令显式的写入的寄存器数组
  - groups: 该条指令所属的指令分组
  - toString(): 把指令格式化成一条人比较容易读懂的字符串形式
  
#### X86Writer
- `new X86Writer(codeAddress[, { pc: ptr('0x1234') }])`：创建一个新的代码编写器，用于生成直接写入内存的 x86 机器代码 codeAddress，指定为NativePointer。第二个参数是一个可选的选项对象，其中可以指定初始程序计数器，这在将代码生成到暂存缓冲区时很有用。这Memory.patchCode() 在 iOS上使用时是必不可少的，它可能会为您提供一个临时位置，该位置稍后会映射到预期内存位置的内存中。
- `reset(codeAddress[, { pc: ptr('0x1234') }])`: 回收实例
- `dispose()`: 急切地清理内存
- `flush()`：解析标签引用并将待处理数据写入内存。一旦完成生成代码，您应该始终调用它。通常也希望在不相关的代码段之间执行此操作，例如在一次性生成多个函数时。
- `base`: 输出第一个字节的内存位置，作为NativePointer
- `code`: 输出的下一个字节的内存位置，作为NativePointer
- `pc`: 在输出的下一个字节处的程序计数器，作为NativePointer
- `offset`: 当前偏移量作为 JavaScript 数字
- `putLabel(id)`: 在当前位置放一个标签，这里id是一个字符串，在过去和未来的put*Label()调用中可能会被引用
- `putCallAddressWithArguments(func, args)`: put 调用带有指定的 C 函数所需的代码args，指定为 JavaScript 数组，其中每个元素要么是指定寄存器的字符串，要么是指定立即值的 Number 或 NativePointer。
- `putCallAddressWithAlignedArguments(func, args)`: 像上面一样，但也确保参数列表在 16 字节边界上对齐
- `putCallRegWithArguments(reg, args)`: put 调用带有指定的 C 函数所需的代码args，指定为 JavaScript 数组，其中每个元素要么是指定寄存器的字符串，要么是指定立即值的 Number 或 NativePointer。
- `putCallRegWithAlignedArguments(reg, args)`: 像上面一样，但也确保参数列表在 16 字节边界上对齐
- `putCallRegOffsetPtrWithArguments(reg, offset, args)`: put 调用带有指定的 C 函数所需的代码args，指定为 JavaScript 数组，其中每个元素要么是指定寄存器的字符串，要么是指定立即值的 Number 或 NativePointer。
- `putCallAddress(address)`: 放置一个 CALL 指令
- `putCallReg(reg)`: 放置一个 CALL 指令
- `putCallRegOffsetPtr(reg, offset)`: 放置一个 CALL 指令
- `putCallIndirect(addr)`: 放置一个 CALL 指令、
- `putCallIndirectLabel(labelId)`: 放置一个 CALL 指令引用labelId，由过去或未来定义putLabel()
- `putCallNearLabel(labelId)`: 放置一个 CALL 指令引用labelId，由过去或未来定义putLabel()
- `putLeave()`: 放一条 LEAVE 指令
- `putRet()`: 放一个RET指令
- `putRetImm(immValue)`: 放一个RET指令
- `putJmpAddress(address)`: 放一条JMP指令
- `putJmpShortLabel(labelId)`: 放置一个 JMP 指令引用labelId，由过去或未来定义putLabel()
- `putJmpNearLabel(labelId)`: 放置一个 JMP 指令引用labelId，由过去或未来定义putLabel()
- `putJmpReg(reg)`: 放一条JMP指令
- `putJmpRegPtr(reg)`: 放一条JMP指令
- `putJmpRegOffsetPtr(reg, offset)`: 放一条JMP指令
- `putJmpNearPtr(address)`: 放一条JMP指令
- `putJccShort(instructionId, target, hint)`: 放一条JCC指令
- `putJccNear(instructionId, target, hint)`: 放一条JCC指令
- `putJccShortLabel(instructionId, labelId, hint)`: 放置一个 JCC 指令引用labelId，由过去或未来定义putLabel()
- `putJccNearLabel(instructionId, labelId, hint)`: 放置一个 JCC 指令引用labelId，由过去或未来定义putLabel()
- `putAddRegImm(reg, immValue)`: 放一条 ADD 指令
- `putAddRegReg(dstReg, srcReg)`: 放一条 ADD 指令
- `putAddRegNearPtr(dstReg, srcAddress)`: 放一条 ADD 指令
- `putSubRegImm(reg, immValue)`: 放置一个 SUB 指令
- `putSubRegReg(dstReg, srcReg)`: 放置一个 SUB 指令
- `putSubRegNearPtr(dstReg, srcAddress)`: 放置一个 SUB 指令
- `putIncReg(reg)`: 放置一个INC指令
- `putDecReg(reg)`: 放一条 DEC 指令
- `putIncRegPtr(target, reg)`: 放置一个INC指令
- `putDecRegPtr(target, reg)`: 放一条 DEC 指令
- putLockXaddRegPtrReg(dstReg, srcReg): 放一个LOCK XADD指令

- putLockCmpxchgRegPtrReg(dstReg, srcReg): 放一个LOCK CMPXCHG 指令

- putLockIncImm32Ptr(target): 放一条LOCK INC IMM32指令

- putLockDecImm32Ptr(target): 放一条LOCK DEC IMM32指令

- putAndRegReg(dstReg, srcReg): 放置一个 AND 指令

- putAndRegU32(reg, immValue): 放置一个 AND 指令

- putShlRegU8(reg, immValue): 放一条SHL指令

- putShrRegU8(reg, immValue): 放一条SHR指令

- putXorRegReg(dstReg, srcReg): 放置一个 XOR 指令

- putMovRegReg(dstReg, srcReg): 放一条 MOV 指令

- putMovRegU32(dstReg, immValue): 放一条 MOV 指令

- putMovRegU64(dstReg, immValue): 放一条 MOV 指令

- putMovRegAddress(dstReg, address): 放一条 MOV 指令

- putMovRegPtrU32(dstReg, immValue): 放一条 MOV 指令

- putMovRegOffsetPtrU32(dstReg, dstOffset, immValue): 放一条 MOV 指令

- putMovRegPtrReg(dstReg, srcReg): 放一条 MOV 指令

- putMovRegOffsetPtrReg(dstReg, dstOffset, srcReg): 放一条 MOV 指令

- putMovRegRegPtr(dstReg, srcReg): 放一条 MOV 指令

- putMovRegRegOffsetPtr(dstReg, srcReg, srcOffset): 放一条 MOV 指令

- putMovRegBaseIndexScaleOffsetPtr(dstReg, baseReg, indexReg, scale, offset): 放一条 MOV 指令

- putMovRegNearPtr(dstReg, srcAddress): 放一条 MOV 指令

- putMovNearPtrReg(dstAddress, srcReg): 放一条 MOV 指令

- putMovFsU32PtrReg(fsOffset, srcReg): 放置一个 MOV FS 指令

- putMovRegFsU32Ptr(dstReg, fsOffset): 放置一个 MOV FS 指令

- putMovGsU32PtrReg(fsOffset, srcReg): 放置一个 MOV GS 指令

- putMovRegGsU32Ptr(dstReg, fsOffset): 放置一个 MOV GS 指令

- putMovqXmm0EspOffsetPtr(offset): 放一条 MOVQ XMM0 ESP 指令

- putMovqEaxOffsetPtrXmm0(offset): 放置一条 MOVQ EAX XMM0 指令

- putMovdquXmm0EspOffsetPtr(offset): 放一条 MOVDQU XMM0 ESP 指令

- putMovdquEaxOffsetPtrXmm0(offset): 放置一条 MOVDQU EAX XMM0 指令

- putLeaRegRegOffset(dstReg, srcReg, srcOffset): 放一条 LEA 指令

- putXchgRegRegPtr(leftReg, rightReg): 放一条XCHG指令

- putPushU32(immValue): 放置一个PUSH指令

- putPushNearPtr(address): 放置一个PUSH指令

- putPushReg(reg): 放置一个PUSH指令

- putPopReg(reg): 放一条POP指令

- putPushImmPtr(immPtr): 放置一个PUSH指令

- putPushax(): 放置一个 PUSHAX 指令

- putPopax(): 放置一个 POPAX 指令

- putPushfx(): 放置一个 PUSHFX 指令

- putPopfx(): 放一个 POPFX 指令

- putTestRegReg(regA, regB): 放一个 TEST 指令

- putTestRegU32(reg, immValue): 放一个 TEST 指令

- putCmpRegI32(reg, immValue): 放一条CMP指令

- utCmpRegOffsetPtrReg(regA, offset, regB): 放一条CMP指令

- putCmpImmPtrImmU32(immPtr, immValue): 放一条CMP指令

- putCmpRegReg(regA, regB): 放一条CMP指令

- putClc(): 放一个 CLC 指令

- putStc(): 放一条STC指令

- putCld(): 放一条CLD指令

- putStd(): 放一条 STD 指令

- putCpuid(): 放一条CPUID指令

- putLfence(): 放置一个 LFENCE 指令

- putRdtsc(): 放一条 RDTSC 指令

- putPause(): 放一条 PAUSE 指令

- putNop(): 放一个 NOP 指令

- putBreakpoint(): 放置一个操作系统/架构特定的断点指令

- putPadding(n): 把n守卫指令

- putNopPadding(n): 放nNOP 指令

- putU8(value): 放一个 uint8

- putS8(value): 放一个 int8

- putBytes(data): 从提供的ArrayBuffer 中放入原始数据

#### X86重定位器
- `new X86Relocator(inputCode, output)`：创建一个新的代码重定位器，用于将 x86 指令从一个内存位置复制到另一个内存位置，注意相应地调整与位置相关的指令。源地址由inputCode一个NativePointer指定。目标由 给出output，一个X86Writer指向所需的目标内存地址。

- `reset(inputCode, output)`: 回收实例

- `dispose()`: 急切地清理内存

- `input`:到目前为止阅读的最新指令。null 在每次调用 时开始并更改readOne()。

- `eob`: boolean 指示是否已到达块尾，即我们已到达任何类型的分支，如 CALL、JMP、BL、RET。

- `eoi`：布尔值，指示是否已到达输入结束，例如，我们已到达 JMP/B/RET，一条指令之后可能有也可能没有有效代码。

- `readOne()`: 将下一条指令读入重定位器的内部缓冲区，并返回到目前为止读取的字节数，包括之前的调用。您可以继续调用此方法以保持缓冲，或者立即调用writeOne()或skipOne()。或者，您可以缓冲到所需的点，然后调用writeAll(). 到达输入结束时返回零，这意味着该eoi属性是 now true。

- `peekNextWriteInsn()`: 偷看下一条要写入或跳过的指令

- `peekNextWriteSource()`: 查看下一条要写入或跳过的指令的地址

- `skipOne()`: 跳过接下来要写的指令

- `skipOneNoLabel()`: 跳过接下来会写的指令，但没有内部使用的标签。这打破了将分支重定位到重定位范围内的位置，并且是对所有分支都被重写的用例的优化（例如 Frida 的Stalker）。

- `writeOne()`: 写下一条缓冲指令

- `writeOneNoLabel()`: 写下一条缓冲指令，但没有内部使用的标签。这打破了将分支重定位到重定位范围内的位置，并且是对所有分支都被重写的用例的优化（例如 Frida 的Stalker）。

- `writeAll()`: 写入所有缓冲指令

#### x86 枚举类型
- 登记： xax xcx xdx xbx xsp xbp xsi xdi eax ecx edx ebx esp ebp esi edi rax rcx rdx rbx rsp rbp rsi rdi r8 r9 r10 r11 r12 r13 r14 r15 r8d r9d r10d r11d r12d r13d r14d r15d xip eip rip
- 指令 ID： jo jno jb jae je jne jbe ja js jns jp jnp jl jge jle jg jcxz jecxz jrcxz
- 分支提示： no-hint likely unlikely
- 指针目标： byte dword qword

#### 写手
- new ArmWriter(codeAddress[, { pc: ptr('0x1234') }])：创建一个新的代码编写器，用于生成直接写入内存的 ARM 机器代码 codeAddress，指定为NativePointer。第二个参数是一个可选的选项对象，其中可以指定初始程序计数器，这在将代码生成到暂存缓冲区时很有用。这Memory.patchCode() 在 iOS上使用时是必不可少的，它可能会为您提供一个临时位置，该位置稍后会映射到预期内存位置的内存中。

- reset(codeAddress[, { pc: ptr('0x1234') }]): 回收实例

- dispose(): 急切地清理内存

- flush()：解析标签引用并将待处理数据写入内存。一旦完成生成代码，您应该始终调用它。通常也希望在不相关的代码段之间执行此操作，例如在一次性生成多个函数时。

- base: 输出第一个字节的内存位置，作为NativePointer

- code: 输出的下一个字节的内存位置，作为NativePointer

- pc: 在输出的下一个字节处的程序计数器，作为NativePointer

- offset: 当前偏移量作为 JavaScript 数字

- skip(nBytes)： 跳过 nBytes

- putLabel(id): 在当前位置放一个标签，这里id是一个字符串，在过去和未来的put*Label()调用中可能会被引用

- putCallAddressWithArguments(func, args): put 调用带有指定的 C 函数所需的代码args，指定为 JavaScript 数组，其中每个元素要么是指定寄存器的字符串，要么是指定立即值的 Number 或 NativePointer。

- putBranchAddress(address): 放置分支/跳转到给定地址所需的代码

- canBranchDirectlyBetween(from, to): 确定在两个给定的内存位置之间是否可能存在直接分支

- putBImm(target): 放一条B指令

- putBCondImm(cc, target): 放置一个 B COND 指令

- putBLabel(labelId): 放置一个 B 指令引用labelId，由过去或未来定义putLabel()

- putBCondLabel(cc, labelId): 放置一个 B COND 指令引用labelId，由过去或未来定义putLabel()

- putBlImm(target): 放一条BL指令

- putBlxImm(target): 放一条BLX指令

- putBlLabel(labelId): 放置一个BL指令引用labelId，由过去或未来定义putLabel()

- putBxReg(reg): 放一条BX指令

- putBlxReg(reg): 放一条BLX指令

- putRet(): 放一个RET指令

- putLdrRegAddress(reg, address): 放一条 LDR 指令

- putLdrRegU32(reg, val): 放一条 LDR 指令

- putLdrRegRegOffset(dstReg, srcReg, srcOffset): 放一条 LDR 指令

- putLdrCondRegRegOffset(cc, dstReg, srcReg, srcOffset): 放置一个 LDR COND 指令

- putLdmiaRegMask(reg, mask): 放置一个 LDMIA MASK 指令

- putStrRegRegOffset(srcReg, dstReg, dstOffset): 放一条STR指令

- putStrCondRegRegOffset(cc, srcReg, dstReg, dstOffset): 放置一个 STR COND 指令

- putMovRegReg(dstReg, srcReg): 放一条 MOV 指令

- putMovRegRegShift(dstReg, srcReg, shift, shiftValue): 放置一个 MOV SHIFT 指令

- putMovRegCpsr(reg): 放置一个 MOV CPSR 指令

- putMovCpsrReg(reg): 放置一个 MOV CPSR 指令

- putAddRegU16(dstReg, val): 放一条 ADD U16 指令

- putAddRegU32(dstReg, val): 放一条 ADD 指令

- putAddRegRegImm(dstReg, srcReg, immVal): 放一条 ADD 指令

- putAddRegRegReg(dstReg, srcReg1, srcReg2): 放一条 ADD 指令

- putAddRegRegRegShift(dstReg, srcReg1, srcReg2, shift, shiftValue): 放置一个 ADD SHIFT 指令

- putSubRegU16(dstReg, val): 放置一个 SUB U16 指令

- putSubRegU32(dstReg, val): 放置一个 SUB 指令

- putSubRegRegImm(dstReg, srcReg, immVal): 放置一个 SUB 指令

- putSubRegRegReg(dstReg, srcReg1, srcReg2): 放置一个 SUB 指令

- putAndsRegRegImm(dstReg, srcReg, immVal): 放置一个 ANDS 指令

- putCmpRegImm(dstReg, immVal): 放一条CMP指令

- putNop(): 放一个 NOP 指令

- putBreakpoint(): 放置一个操作系统/架构特定的断点指令

- putBrkImm(imm): 放一条 BRK 指令

- putInstruction(insn): 将原始指令作为 JavaScript 编号

- putBytes(data): 从提供的ArrayBuffer 中放入原始数据

#### 臂重定位器
- `new ArmRelocator(inputCode, output)`：创建一个新的代码重定位器，用于将 ARM 指令从一个内存位置复制到另一个内存位置，注意相应地调整位置相关的指令。源地址由inputCode一个NativePointer指定。目标由output，一个指向所需目标内存地址的ArmWriter给出。

- `reset(inputCode, output)`: 回收实例

- `dispose()`: 急切地清理内存

- `input`:到目前为止阅读的最新指令。null 在每次调用 时开始并更改readOne()。

- `eob`: boolean 指示是否已到达块尾，即我们已到达任何类型的分支，如 CALL、JMP、BL、RET。

- `eoi`：布尔值，指示是否已到达输入结束，例如，我们已到达 JMP/B/RET，一条指令之后可能有也可能没有有效代码。

- `readOne()`: 将下一条指令读入重定位器的内部缓冲区，并返回到目前为止读取的字节数，包括之前的调用。您可以继续调用此方法以保持缓冲，或者立即调用writeOne()或skipOne()。或者，您可以缓冲到所需的点，然后调用writeAll(). 到达输入结束时返回零，这意味着该eoi属性是 now true。

- `peekNextWriteInsn()`: 偷看下一条要写入或跳过的指令

- `peekNextWriteSource()`: 查看下一条要写入或跳过的指令的地址

- `skipOne()`: 跳过接下来要写的指令

- `writeOne()`: 写下一条缓冲指令

- `writeAll()`: 写入所有缓冲指令

#### 拇指作家
- new ThumbWriter(codeAddress[, { pc: ptr('0x1234') }])：创建一个新的代码编写器，用于生成直接写入内存的 ARM 机器代码 codeAddress，指定为NativePointer。第二个参数是一个可选的选项对象，其中可以指定初始程序计数器，这在将代码生成到暂存缓冲区时很有用。这Memory.patchCode() 在 iOS上使用时是必不可少的，它可能会为您提供一个临时位置，该位置稍后会映射到预期内存位置的内存中。

- reset(codeAddress[, { pc: ptr('0x1234') }]): 回收实例

- dispose(): 急切地清理内存

- flush()：解析标签引用并将待处理数据写入内存。一旦完成生成代码，您应该始终调用它。通常也希望在不相关的代码段之间执行此操作，例如在一次性生成多个函数时。

- base: 输出第一个字节的内存位置，作为NativePointer

- code: 输出的下一个字节的内存位置，作为NativePointer

- pc: 在输出的下一个字节处的程序计数器，作为NativePointer

- offset: 当前偏移量作为 JavaScript 数字

- skip(nBytes)： 跳过 nBytes

- putLabel(id): 在当前位置放一个标签，这里id是一个字符串，在过去和未来的put*Label()调用中可能会被引用

- commitLabel(id): 提交对给定标签的第一个待处理引用，true成功返回。返回false如果给定的标签尚未定义的呢，还是有它没有更多的待定引用。

- putCallAddressWithArguments(func, args): put 调用带有指定的 C 函数所需的代码args，指定为 JavaScript 数组，其中每个元素要么是指定寄存器的字符串，要么是指定立即值的 Number 或 NativePointer。

- putCallRegWithArguments(reg, args): put 调用带有指定的 C 函数所需的代码args，指定为 JavaScript 数组，其中每个元素要么是指定寄存器的字符串，要么是指定立即值的 Number 或 NativePointer。

- putBImm(target): 放一条B指令

- putBLabel(labelId): 放置一个 B 指令引用labelId，由过去或未来定义putLabel()

- putBLabelWide(labelId): 放一条 B WIDE 指令

- putBxReg(reg): 放一条BX指令

- putBlImm(target): 放一条BL指令

- putBlLabel(labelId): 放置一个BL指令引用labelId，由过去或未来定义putLabel()

- putBlxImm(target): 放一条BLX指令

- putBlxReg(reg): 放一条BLX指令

- putCmpRegImm(reg, immValue): 放一条CMP指令

- putBeqLabel(labelId): 放置一个 BEQ 指令引用labelId，由过去或未来定义putLabel()

- putBneLabel(labelId): 放置一个 BNE 指令引用labelId，由过去或未来定义putLabel()

- putBCondLabel(cc, labelId): 放置一个 B COND 指令引用labelId，由过去或未来定义putLabel()

- putBCondLabelWide(cc, labelId): 放置一个 B COND WIDE 指令

- putCbzRegLabel(reg, labelId): 放置一个 CBZ 指令引用labelId，由过去或未来定义putLabel()

- putCbnzRegLabel(reg, labelId): 放置一个 CBNZ 指令引用labelId，由过去或未来定义putLabel()

- putPushRegs(regs): 使用指定的寄存器放置 PUSH 指令，指定为 JavaScript 数组，其中每个元素都是指定寄存器名称的字符串。

- putPopRegs(regs): 使用指定的寄存器放置 POP 指令，指定为 JavaScript 数组，其中每个元素都是指定寄存器名称的字符串。

- putLdrRegAddress(reg, address): 放一条 LDR 指令

- putLdrRegU32(reg, val): 放一条 LDR 指令

- putLdrRegReg(dstReg, srcReg): 放一条 LDR 指令

- putLdrRegRegOffset(dstReg, srcReg, srcOffset): 放一条 LDR 指令

- putLdrbRegReg(dstReg, srcReg): 放一条LDRB指令

- putVldrRegRegOffset(dstReg, srcReg, srcOffset): 放一条VLDR指令

- putLdmiaRegMask(reg, mask): 放置一个 LDMIA MASK 指令

- putStrRegReg(srcReg, dstReg): 放一条STR指令

- putStrRegRegOffset(srcReg, dstReg, dstOffset): 放一条STR指令

- putMovRegReg(dstReg, srcReg): 放一条 MOV 指令

- putMovRegU8(dstReg, immValue): 放一条 MOV 指令

- putMovRegCpsr(reg): 放置一个 MOV CPSR 指令

- putMovCpsrReg(reg): 放置一个 MOV CPSR 指令

- putAddRegImm(dstReg, immValue): 放一条 ADD 指令

putAddRegReg(dstReg, srcReg): 放一条 ADD 指令

putAddRegRegReg(dstReg, leftReg, rightReg): 放一条 ADD 指令

putAddRegRegImm(dstReg, leftReg, rightValue): 放一条 ADD 指令

putSubRegImm(dstReg, immValue): 放置一个 SUB 指令

putSubRegReg(dstReg, srcReg): 放置一个 SUB 指令

putSubRegRegReg(dstReg, leftReg, rightReg): 放置一个 SUB 指令

putSubRegRegImm(dstReg, leftReg, rightValue): 放置一个 SUB 指令

putAndRegRegImm(dstReg, leftReg, rightValue): 放置一个 AND 指令

putLslsRegRegImm(dstReg, leftReg, rightValue): 放一条 LSLS 指令

putLsrsRegRegImm(dstReg, leftReg, rightValue): 放一条 LSRS 指令

putMrsRegReg(dstReg, srcReg): 放一条 MRS 指令

putMsrRegReg(dstReg, srcReg): 放一条 MSR 指令

putNop(): 放一个 NOP 指令

- putBkptImm(imm): 放一条BKPT指令

- putBreakpoint(): 放置一个操作系统/架构特定的断点指令

- putInstruction(insn): 将原始指令作为 JavaScript 编号

- putInstructionWide(upper, lower): 从两个 JavaScript Number 值中放置一个原始的 Thumb-2 指令

- putBytes(data): 从提供的ArrayBuffer 中放入原始数据

#### 拇指重定位器
- new ThumbRelocator(inputCode, output)：创建一个新的代码重定位器，用于将 ARM 指令从一个内存位置复制到另一个内存位置，注意相应地调整位置相关的指令。源地址由inputCode一个NativePointer指定。目标由output，一个指向所需目标内存地址的ThumbWriter给出。

- reset(inputCode, output): 回收实例

- dispose(): 急切地清理内存

- input:到目前为止阅读的最新指令。null 在每次调用 时开始并更改readOne()。

- eob: boolean 指示是否已到达块尾，即我们已到达任何类型的分支，如 CALL、JMP、BL、RET。

- eoi：布尔值，指示是否已到达输入结束，例如，我们已到达 JMP/B/RET，一条指令之后可能有也可能没有有效代码。

- readOne(): 将下一条指令读入重定位器的内部缓冲区，并返回到目前为止读取的字节数，包括之前的调用。您可以继续调用此方法以保持缓冲，或者立即调用writeOne()或skipOne()。或者，您可以缓冲到所需的点，然后调用writeAll(). 到达输入结束时返回零，这意味着该eoi属性是 now true。

- peekNextWriteInsn(): 偷看下一条要写入或跳过的指令

- peekNextWriteSource(): 查看下一条要写入或跳过的指令的地址

- skipOne(): 跳过接下来要写的指令

- writeOne(): 写下一条缓冲指令

- copyOne(): 复制下一条缓冲指令而不前进输出光标，允许多次写出同一条指令

- writeAll(): 写入所有缓冲指令

#### ARM 枚举类型
- 登记： r0 r1 r2 r3 r4 r5 r6 r7 r8 r9 r10 r11 r12 r13 r14 r15 sp lr sb sl fp ip pc
- 系统注册： apsr-nzcvq
- 条件代码： eq ne hs lo mi pl vs vc hi ls ge lt gt le al
- 变速杆： asr lsl lsr ror rrx asr-reg lsl-reg lsr-reg ror-reg rrx-reg

#### Arm64Writer
- `new Arm64Writer(codeAddress[, { pc: ptr('0x1234') }])`：创建一个新的代码编写器，用于生成直接写入内存的 AArch64 机器代码 codeAddress，指定为NativePointer。第二个参数是一个可选的选项对象，其中可以指定初始程序计数器，这在将代码生成到暂存缓冲区时很有用。这Memory.patchCode() 在 iOS上使用时是必不可少的，它可能会为您提供一个临时位置，该位置稍后会映射到预期内存位置的内存中。

- `reset(codeAddress[, { pc: ptr('0x1234') }])`: 回收实例

- `dispose()`: 急切地清理内存

- flush()：解析标签引用并将待处理数据写入内存。一旦完成生成代码，您应该始终调用它。通常也希望在不相关的代码段之间执行此操作，例如在一次性生成多个函数时。

- base: 输出第一个字节的内存位置，作为 NativePointer

- code: 输出的下一个字节的内存位置，作为 NativePointer

- pc: 在输出的下一个字节处的程序计数器，作为 NativePointer

- offset: 当前偏移量作为 JavaScript 数字

- skip(nBytes)： 跳过 nBytes

- putLabel(id): 在当前位置放一个标签，这里id是一个字符串，在过去和未来的put*Label()调用中可能会被引用

- putCallAddressWithArguments(func, args): put 调用带有指定的 C 函数所需的代码args，指定为 JavaScript 数组，其中每个元素要么是指定寄存器的字符串，要么是指定立即值的 Number 或 NativePointer。

- putCallRegWithArguments(reg, args): put 调用带有指定的 C 函数所需的代码args，指定为 JavaScript 数组，其中每个元素要么是指定寄存器的字符串，要么是指定立即值的 Number 或 NativePointer。

- putBranchAddress(address): 放置分支/跳转到给定地址所需的代码

- canBranchDirectlyBetween(from, to): 确定在两个给定的内存位置之间是否可能存在直接分支

- putBImm(address): 放一条B指令

- putBLabel(labelId): 放置一个 B 指令引用labelId，由过去或未来定义putLabel()

- putBCondLabel(cc, labelId): 放置一个 B COND 指令引用labelId，由过去或未来定义putLabel()

- putBlImm(address): 放一条BL指令

- putBlLabel(labelId): 放置一个BL指令引用labelId，由过去或未来定义putLabel()

- putBrReg(reg): 放一个 BR 指令

- putBrRegNoAuth(reg): 放置一个 BR 指令，期望一个没有任何认证位的原始指针

- putBlrReg(reg): 放一条 BLR 指令

- putBlrRegNoAuth(reg): 放置一个 BLR 指令，期望一个没有任何认证位的原始指针

- putRet(): 放一个RET指令

- putCbzRegLabel(reg, labelId): 放置一个 CBZ 指令引用labelId，由过去或未来定义putLabel()

- putCbnzRegLabel(reg, labelId): 放置一个 CBNZ 指令引用labelId，由过去或未来定义putLabel()

- putTbzRegImmLabel(reg, bit, labelId): 放置一个 TBZ 指令引用labelId，由过去或未来定义putLabel()

- putTbnzRegImmLabel(reg, bit, labelId): 放置一个 TBNZ 指令引用labelId，由过去或未来定义putLabel()

- putPushRegReg(regA, regB): 放置一个PUSH指令

- putPopRegReg(regA, regB): 放一条POP指令

- putPushAllXRegisters(): 将所有 X 寄存器压入堆栈所需的代码

- putPopAllXRegisters(): 将所有 X 寄存器从堆栈中弹出所需的代码

- putPushAllQRegisters(): 把将所有 Q 寄存器压入堆栈所需的代码

- putPopAllQRegisters(): 将所有 Q 寄存器从堆栈中弹出所需的代码

- putLdrRegAddress(reg, address): 放一条 LDR 指令

- putLdrRegU64(reg, val): 放一条 LDR 指令

- putLdrRegRef(reg): 放置一个带有悬空数据引用的 LDR 指令，返回一个不透明的引用值，该值应该传递到putLdrRegValue() 所需的位置

- putLdrRegValue(ref, value): 输入值并更新前一个 LDR 指令 putLdrRegRef()

- putLdrRegRegOffset(dstReg, srcReg, srcOffset): 放一条 LDR 指令

- putLdrswRegRegOffset(dstReg, srcReg, srcOffset): 放一条 LDRSW 指令

- putAdrpRegAddress(reg, address): 放一条 ADRP 指令

- putStrRegRegOffset(srcReg, dstReg, dstOffset): 放一条STR指令

- putLdpRegRegRegOffset(regA, regB, regSrc, srcOffset, mode): 放置一条 LDP 指令

- putStpRegRegRegOffset(regA, regB, regDst, dstOffset, mode): 放一条STP指令

- putMovRegReg(dstReg, srcReg): 放一条 MOV 指令

- putUxtwRegReg(dstReg, srcReg): 放置一个 UXTW 指令

- putAddRegRegImm(dstReg, leftReg, rightValue): 放一条 ADD 指令

- putAddRegRegReg(dstReg, leftReg, rightReg): 放一条 ADD 指令

- putSubRegRegImm(dstReg, leftReg, rightValue): 放置一个 SUB 指令

- putSubRegRegReg(dstReg, leftReg, rightReg): 放置一个 SUB 指令

- putAndRegRegImm(dstReg, leftReg, rightValue): 放置一个 AND 指令

- putTstRegImm(reg, immValue): 放一条 TST 指令

- putCmpRegReg(regA, regB): 放一条CMP指令

- putXpaciReg(reg): 放一条XPACI指令

- putNop(): 放一个 NOP 指令

- putBrkImm(imm): 放一条 BRK 指令

- putInstruction(insn): 将原始指令作为 JavaScript 编号

- putBytes(data): 从提供的ArrayBuffer 中放入原始数据

- sign(value): 给给定的指针值签名

#### Arm64重定位器
- `new Arm64Relocator(inputCode, output)`：创建一个新的代码重定位器，用于将 AArch64 指令从一个内存位置复制到另一个内存位置，注意相应地调整与位置相关的指令。源地址由inputCode一个NativePointer指定。目标由指向所需目标内存地址output的Arm64Writer给出。

- `reset(inputCode, output)`: 回收实例

- `dispose()`: 急切地清理内存

- input:到目前为止阅读的最新指令。null 在每次调用 时开始并更改readOne()。

- eob: boolean 指示是否已到达块尾，即我们已到达任何类型的分支，如 CALL、JMP、BL、RET。

- eoi：布尔值，指示是否已到达输入结束，例如，我们已到达 JMP/B/RET，一条指令之后可能有也可能没有有效代码。

- readOne(): 将下一条指令读入重定位器的内部缓冲区，并返回到目前为止读取的字节数，包括之前的调用。您可以继续调用此方法以保持缓冲，或者立即调用writeOne()或skipOne()。或者，您可以缓冲到所需的点，然后调用writeAll(). 到达输入结束时返回零，这意味着该eoi属性是 now true。

- peekNextWriteInsn(): 偷看下一条要写入或跳过的指令

- peekNextWriteSource(): 查看下一条要写入或跳过的指令的地址

- skipOne(): 跳过接下来要写的指令

- writeOne(): 写下一条缓冲指令

- writeAll(): 写入所有缓冲指令

#### AArch64 枚举类型
- 登记： x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 x10 x11 x12 x13 x14 x15 x16 x17 x18 x19 x20 x21 x22 x23 x24 x25 x26 x27 x28 x29 x30 w0 w1 w2 w3 w4 w5 w6 w7 w8 w9 w10 w11 w12 w13 w14 w15 w16 w17 w18 w19 w20 w21 w22 w23 w24 w25 w26 w27 w28 w29 w30 sp lr fp wsp wzr xzr nzcv ip0 ip1 s0 s1 s2 s3 s4 s5 s6 s7 s8 s9 s10 s11 s12 s13 s14 s15 s16 s17 s18 s19 s20 s21 s22 s23 s24 s25 s26 s27 s28 s29 s30 s31 d0 d1 d2 d3 d4 d5 d6 d7 d8 d9 d10 d11 d12 d13 d14 d15 d16 d17 d18 d19 d20 d21 d22 d23 d24 d25 d26 d27 d28 d29 d30 d31 q0 q1 q2 q3 q4 q5 q6 q7 q8 q9 q10 q11 q12 q13 q14 q15 q16 q17 q18 q19 q20 q21 q22 q23 q24 q25 q26 q27 q28 q29 q30 q31

- 条件代码： eq ne hs lo mi pl vs vc hi ls ge lt gt le al nv

- 索引模式： post-adjust signed-offset pre-adjust

#### MipsWriter
- new MipsWriter(codeAddress[, { pc: ptr('0x1234') }])：创建一个新的代码编写器，用于生成直接写入内存的 MIPS 机器代码 codeAddress，指定为NativePointer。第二个参数是一个可选的选项对象，其中可以指定初始程序计数器，这在将代码生成到暂存缓冲区时很有用。这Memory.patchCode() 在 iOS上使用时是必不可少的，它可能会为您提供一个临时位置，该位置稍后会映射到预期内存位置的内存中。

- reset(codeAddress[, { pc: ptr('0x1234') }]): 回收实例

- dispose(): 急切地清理内存

- flush()：解析标签引用并将待处理数据写入内存。一旦完成生成代码，您应该始终调用它。通常也希望在不相关的代码段之间执行此操作，例如在一次性生成多个函数时。

- base: 输出第一个字节的内存位置，作为NativePointer

- code: 输出的下一个字节的内存位置，作为NativePointer

- pc: 在输出的下一个字节处的程序计数器，作为NativePointer

- offset: 当前偏移量作为 JavaScript 数字

- skip(nBytes)： 跳过 nBytes

- putLabel(id): 在当前位置放一个标签，这里id是一个字符串，在过去和- 未来的put*Label()调用中可能会被引用

- putCallAddressWithArguments(func, args): put 调用带有指定的 C 函数所需的代码args，指定为 JavaScript 数组，其中每个元素要么是指定寄存器的字符串，要么是指定立即值的 Number 或 NativePointer。

- putCallRegWithArguments(reg, args): put 调用带有指定的 C 函数所需的代码args，指定为 JavaScript 数组，其中每个元素要么是指定寄存器的字符串，要么是指定立即值的 Number 或 NativePointer。

- putJAddress(address): 放一条J指令

- putJAddressWithoutNop(address): 放一个 J WITHOUT NOP 指令

- putJLabel(labelId): 放置一个 J 指令引用labelId，由过去或未来定义putLabel()

- putJrReg(reg): 放一条JR指令

- putJalAddress(address): 放一条 JAL 指令

- putJalrReg(reg): 放一条 JALR 指令

- putBOffset(offset): 放一条B指令

- putBeqRegRegLabel(rightReg, leftReg, labelId): 放置一个 BEQ 指令引用labelId，由过去或未来定义putLabel()

- putRet(): 放一个RET指令

- putLaRegAddress(reg, address): 放一条 LA 指令

- putLuiRegImm(reg, imm): 放一条LUI指令

- putDsllRegReg(dstReg, srcReg, amount): 放一条DSLL指令

- putOriRegRegImm(rt, rs, imm): 放置一个 ORI 指令

- putLdRegRegOffset(dstReg, srcReg, srcOffset): 放一条LD指令

- putLwRegRegOffset(dstReg, srcReg, srcOffset): 放一条LW指令

- putSwRegRegOffset(srcReg, dstReg, dstOffset): 放一条SW指令

- putMoveRegReg(dstReg, srcReg): 放置一个 MOVE 指令

- putAdduRegRegReg(dstReg, leftReg, rightReg): 放一条 ADDU 指令

- putAddiRegRegImm(dstReg, leftReg, imm): 放一条 ADDI 指令

- putAddiRegImm(dstReg, imm): 放一条 ADDI 指令

- putSubRegRegImm(dstReg, leftReg, imm): 放置一个 SUB 指令

- putPushReg(reg): 放置一个PUSH指令

- putPopReg(reg): 放一条POP指令

- putMfhiReg(reg): 放一条MFHI指令

- putMfloReg(reg): 放一条MFLO指令

- putMthiReg(reg): 放一条 MTHI 指令

- putMtloReg(reg): 放一条 MTLO 指令

- putNop(): 放一个 NOP 指令

- putBreak(): 放一条 BREAK 指令

- putPrologueTrampoline(reg, address): 放置一个最小尺寸的蹦床，用于引导到给定地址

- putInstruction(insn): 将原始指令作为 JavaScript 编号

- putBytes(data): 从提供的ArrayBuffer 中放入原始数据

#### Mips重定位器
- new MipsRelocator(inputCode, output)：创建一个新的代码重定位器，用于将 MIPS 指令从一个内存位置复制到另一个内存位置，注意相应地调整与位置相关的指令。源地址由inputCode一个NativePointer指定。目标由output，指向所需目标内存地址的MipsWriter给出。

- reset(inputCode, output): 回收实例

- dispose(): 急切地清理内存

- input:到目前为止阅读的最新指令。null 在每次调用 时开始并更改readOne()。

- eob: boolean 指示是否已到达块尾，即我们已到达任何类型的分支，如 CALL、JMP、BL、RET。

- eoi：布尔值，指示是否已到达输入结束，例如，我们已到达 JMP/B/RET，一条指令之后可能有也可能没有有效代码。

- readOne(): 将下一条指令读入重定位器的内部缓冲区，并返回到目前为止读取的字节数，包括之前的调用。您可以继续调用此方法以保持缓冲，或者立即调用writeOne()或skipOne()。或者，您可以缓冲到所需的点，然后调用writeAll(). 到达输入结束时返回零，这意味着该eoi属性是 now true。

- peekNextWriteInsn(): 偷看下一条要写入或跳过的指令

- peekNextWriteSource(): 查看下一条要写入或跳过的指令的地址

- skipOne(): 跳过接下来要写的指令

- writeOne(): 写下一条缓冲指令

- writeAll(): 写入所有缓冲指令

#### MIPS 枚举类型
- 登记： v0 v1 a0 a1 a2 a3 t0 t1 t2 t3 t4 t5 t6 t7 s0 s1 s2 s3 s4 s5 s6 s7 t8 t9 k0 k1 gp sp fp s8 ra hi lo zero at 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31

#### console
- `console.log(line), console.warn(line), console.error(line)` 向标准输入输出界面写入line字符串，比如：使用Frida-Python的时候就输出到 stdout 或者 stderr，使用frida-qml的时候则输出到qDebug，如果输出的是一个ArrayBuffer，会以默认参数自动调用hexdump进行格式化输出。

ArrayBuffer对象的参数将被hexdump()默认选项的结果替换。

#### 十六进制转储
- `hexdump(target[, options])` 把一个ArrayBuffer或者NativePointer的target变量, 按照附加参数option指定的格式输出

例如：
```javascript
const libc = Module.findBaseAddress('libc.so');
console.log(hexdump(libc, {
  offset: 0,
  length: 64,
  header: true,
  ansi: true
}));
```
```
           0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F  0123456789ABCDEF
00000000  7f 45 4c 46 01 01 01 00 00 00 00 00 00 00 00 00  .ELF............
00000010  03 00 28 00 01 00 00 00 00 00 00 00 34 00 00 00  ..(.........4...
00000020  34 a8 04 00 00 00 00 05 34 00 20 00 08 00 28 00  4.......4. ...(.
00000030  1e 00 1d 00 06 00 00 00 34 00 00 00 34 00 00 00  ........4...4...
```

#### Global
- `int64(v)` new Int64(v)的缩写格式
- `uint64(v)` new UInt64(v)的缩写格式
- `ptr(s)` new NativePointer(s)的缩写格式
- `NULL` ptr("0")的缩写格式

#### 主机和注入进程之间的通信
- `recv([type, ]callback)`：注册回调callback, 当接受到消息时, 会指向回调函数，可选参数type用于对消息的过滤，只接受对应type的消息，当收到一个消息后，如果需要继续接受消息，需要再次调用recv
- `send(message[, data])` 向目标进程发送message(必须可序列化为Json), 如果还有附加数据需要发送，可以使用data(数据类型必须是ArrayBuffer或者为整形数组(数值为0-255)) 虽然send()是异步的，但发送单个消息的总开销并未针对高频进行优化，因此可以将多个消息暂存批量发送，从而获取更低的延时和更高的吞吐量。
- `rpc.exports` 可以在程序中导出RPC风格的APi函数, Key指定导出的名称，Value为指定导出的函数，函数可以直接返回一个值，也可以是异步方式以Promise的方式返回。
例如：
```javascript
rpc.exports = {
  add: function(a, b) {
    return a + b;
  },
  sub: function(a, b) {
    return new Promise(resolve => {
      setTimeout(() => {
        resolve(a - b);
      }, 100);
    });
  }
};
```

对于Python主控端可以使用下面这样的脚本使用导出的RPC函数：
```python
import codecs
import frida

def on_message(message, data):
    if message['type'] == 'send':
        print(message['payload'])
    elif message['type'] == 'error':
        print(message['stack'])

session = frida.attach('iTunes')
with codecs.open('./agent.js', 'r', 'utf-8') as f:
    source = f.read()
script = session.create_script(source)
script.on('message', on_message)
script.load()
print(script.exports.add(2, 3))
print(script.exports.sub(5, 3))
session.detach()
```
在上面的示例中，我们使用`script.on('message', on_message)`监视来自目标进程的任何消息。消息监控可以来自`script`和`session`两个方面。比如，如果您想监控目标进程退出时，可以使用 `session.on('detached', your_function)`.

#### 定时事件
- `setTimeout(func, delay[, ...parameters])` 延迟delay毫秒后调用func, 可以选择传递一个或多个parameters。返回值为一个ID，该ID可以传递给clearTimeout取消对func函数的调用
- `clearTimeout(id)` 取消通过setTimeout发起的调用
- `setInterval(func, delay[, ...parameters])`:每delay毫秒调用一次func，可以选择传递一个或多个parameters。返回值为一个ID, 该ID可以传递clearInterval取消对func函数的调用
- `clearInterval(id)` 取消通过setTimeout发起的调用
- `setImmediate(func[, ...parameters])`: 尽快在Frida的JavaScript线程上调用，可选择传递一个或多个parameters。返回值为一个ID, 该ID可以传递clearImmediate取消对func函数的调用
- `clearImmediate(id)` 取消通过setImmediate发起的调用

#### 垃圾回收
- `gc()`: 强制垃圾回收。对测试很有用，尤其是涉及`WeakRef.bind()`.












































