
# Frida API�����ֲ�

## Frida

- `Frida.version` Frida�汾��Ϣ String
- `Frida.heapsize` Frida˽�жѵĴ�С Number

## Script

- `Script.runtime` Frida����ʱ��, ������QJS��V8

## Thread

- `Thread.backtrace([context, backtracer])` ץȡ��ǰ�̵߳ĵ��ö�ջ������ NativePointer ָ���������ʽ���ء���������� Interceptor.onEnter����Interceptor.onLeave �е�����������Ļ����Ǿͱ���Ҫ�� this.context ��Ϊ�������룬���������õ����Ѿ�׼�Ķ�ջ������Ϣ�����ʡ����������������Ǿ���ζ�Ŵӵ�ǰ��ջ��λ�ÿ�ʼץȡ��������ץȡЧ�����ܲ���ܺã���Ϊ�в���V8�����ջ֡�ĸ��š��ڶ�����ѡ���� backtracer����ʾʹ���������͵Ķ�ջץȡ�㷨��Ŀǰ��ȡֵ��Χ�� Backtracer.FUZZY �� Backtracer.ACCURATE��Ŀǰ������Ĭ��ģʽ����ȷץȡģʽ�£���������ǵ������Ѻã������Ǳ�׼����������Ľ����û��ʲô�����ԣ������з��ű��֧�֣�ץȡЧ������õģ���ģ��ץȡģʽ�£�ץȡ�����ڶ�ջ�ϳ���ץȡ�����һ�²���������ķ��ص�ַ��Ҳ����˵�м���ܰ���һЩ�������Ϣ����������ģʽ���������κζ����Ƴ������湤����

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

- `Thread.sleep(delay)` ����ǰ�̵߳�ִ�У���ʱdelay������ִ��

### Process

- `Process.id` ��ǰ����ID Number
- `Process.arch` ���������� String (ia32 | X64 | arm | arm64)
- `Process.platform` ����ϵͳ���� String (windows | linux | darwin | qnx)
- `Process.PageSize` ����ҳ��Ĵ�С Number
- `Process.pointerSize` ָ�����͵Ĵ�С Number
- `Process.codeSigningPolicy` ����ǩ������
- `Process.isDebuggerAttached()` �����Ƿ񱻵��� boolean
- `Process.getCurrentThreadId()` ��ǰ�߳�ID Number
- `Process.enumerateThreads()` ö�ٽ����������߳� thread array objects
  - `thread.id` �߳�id Number
  - `thread.state` �̵߳�״̬ String (running | stopped | waiting | uninterruptible | halted) String
  - `thread.context` �߳�������(EIP/RIP/PC and ESP/RSP/SP) NativePointer
- `Process.enumerateThreadsSync(callbacks)` Process.enumerateThreads(callbacks)ͬ���汾
- `Process.findModuleByAddress(address) | Process.getModuleByAddress(address) | Process.findModuleByName(name) | Process.getModuleByName(name)` ��ȡ��ַ��Ӧ��ģ����Ϣ Module �����ڸ�ģ�� findǰ׺�᷵��null getǰ׺���׳��쳣
- `Process.enumerateModules()` ��ȡ���̼��ص�����ģ�� module array
  - `module.name` ģ������
  - `module.base` ģ���ַ
  - `module.size` ģ���С
  - `module.path` ģ��·��
- `Process.enumerateModulesSync()` enumerateModules()������ͬ���汾������ģ���������
- `Process.findRangeByAddress(address) | getRangeByAddress(address)` �����йذ�����ַ�ķ�Χ����Ϣ object, ��Ӧ�ֶβο�
- `Process.enumerateRanges(protection|specifier)` ö��ָ������protoction���ڴ�飬��ָ���ַ�������"rwx", "rw-"��ʶ���ٿɶ���д��Ҳ����ʹ�÷����, �������protection���Key��ȡֵ����ǰ���ᵽ��rwx������һ��coalesce���Key����ʾ�Ƿ�Ҫ��λ�����ڲ���������ͬ���ڴ��ϲ��������
  - `onMatch: function(range)` ÿ��ö�ٵ�һ���ڴ�鶼�ص����������Ҫֹͣö�ٹ��̣�ֱ�ӷ���stop����.����Range��������������ԣ�
    - `range.base` ����ַ
    - `range.size` �ڴ���С
    - `range.protection` ��������
    - `range.file`������еĻ����ڴ�ӳ���ļ���
      - `file.path` �ļ�·��
      - `file.offset` �ļ���ƫ��
  - `onComplete: function()` �����ڴ��ö�����֮���ص�
- `Process.setExceptionHandler(callback)` �����쳣����ص������лص�������һ������. �����쳣֮����ôʹ�þͿ����Լ��ˣ�������԰��쳣��Ϣд����־���棬Ȼ���͸���Ϣ�����ضˣ�Ȼ��ͬ���ȴ����ض˵���Ӧ֮��������ֱ���޸��쳣��Ϣ��������ļĴ�����ֵ�����Իָ����쳣������ִ�С�����㴦�����쳣��Ϣ����ô����쳣�ص�������Ҫ���� true��Frida����쳣���������쳣������������������û��ȥ��������쳣����ֱ�ӽ���Ŀ����̡�arg: details
  - `type` �쳣���� String (abort access-violation guard-page �Ƿ������ڴ�| illegal-instruction �Ƿ�ָ�� | stack-overflow ջ��� | arithmetic �����쳣 |breakpoint �ϵ� |  single-step ���� | system ϵͳ����)
  - `address` �쳣��ַ NativePointer
  - `memory`  ����������ǿ� object containing:
  - `memory.operation` �����쳣������ String(read | write | execute)
  - `memory.address` �쳣�����ĵ�ַ NativePointer
  - `context` �쳣�������߳���������Ϣ NativePointer objects
      �������У�pc��sp�Ƿֱ�Ϊia32/x64/armָ��EIP/RIP/pc��ESP/RSP/sp�ı���ָ����������������ض���Ҳ���ã�����eax��rax��r0��x0�ȡ���Ҳ����ͨ���������Щ�������¼Ĵ���ֵ��
  - `nativeContext` NativePointer

  ``` shell
    ## ʹ��frida-ps ���ҽ���
    [root@yanglk fridastest]# frida-ps | grep main
    43280  main  
    ## ���ӵ�ָ������           
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
    ## �鿴����������                                                                            
    [Local::PID::43280]-> Process.arch
    "x64"
    ## �鿴����ID
    [Local::PID::43280]-> Process.id
    43280
    ## �鿴ϵͳƽ̨
    [Local::PID::43280]-> Process.platform
    "linux"
    ## �鿴����ǩ������
    [Local::PID::43280]-> Process.codeSigningPolicy
    "optional"
    ##1 �鿴�����Ƿ񱻵���
    [Local::PID::43280]-> Process.isDebuggerAttached()
    false
    ##2 ʹ��gdb���е���
    gdb -p 43280
    ##3 �ٴβ鿴�����Ƿ񱻵���
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
    ## ö�ٽ����ڲ������߳�
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
    ## ö�ٽ������м��ص�ģ��
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

  Module.load() and Process.enumerateModules()�Ⱥ������ú󷵻�Module�������а���������

- `Module.name` ģ������ String
- `Module.base` ģ�����ַ NativePointer
- `Module.size` ģ���С size in bytes
- `Module.path` ģ�����ļ�ϵͳ�е�����·�� String
- `Module.enumerateImports()` ö��ģ��ĵ������Ϣ, object array
  - `onMatch: function(imp)` ö�ٵ�һ�������ʱ��ᱻ���ã�imp�������µ��ֶ�, �������е������ֶΣ�ֻ��name�ֶ���һ�����У�ʣ��������ֶβ��ܱ�֤���У��ײ�ᾡ����֤ÿ���ֶζ��ܸ������ݣ����ǲ��ܱ�֤һ�����õ����ݣ�onMatch�������Է����ַ���stop��ʾҪֹͣö�١�
    - `imp.type` ������е����� (function | variable) String
    - `imp.name`������Ϣ String
    - `imp.module` ģ������ String
    - `imp.address`���Ե�ַ NativePointer
    - `imp.slot` ��������ڵ�ַ NativePointer
  - `onComplete: function()` �����еĵ�����ö����ɵ�ʱ���ص�
- `Module.eumerateImportsSync(name)` enumerateImports()��ͬ���汾
- `Module.enumerateExports(name, callbacks)` ö��ָ��ģ��name�ĵ�����������callbacks���лص�
  - `onMatch: function(exp)` ��������stop��ʱ���ʾֹͣö�ٹ��̣�����exp����ö�ٵ���һ��������������¼����ֶ�
    - `exp.type` �������е����� (function | variable) String
    - `exp.name` ������Ϣ String
    - `exp.address` ���Ե�ַ NativePointer
  - `onComplete: function()` ö�����ʱ�ص�
- `Module.enumerateExportsSync()` Module.enumerateExports()��ͬ���汾
- `Module.enumerateSymbols(name, callbacks)` ö��ָ��ģ���а����ķ��ţ�ö�ٽ��ͨ���ص�����֪ͨ
  - `onMatch: function(sym)` ���� sym �������漸���ֶ�
    - `sym.isGlobal` ȫ�ֿɼ���ʶ boolean
    - `sym.type` ���� (unknown | section | undefined(Mach-O) | absolute (Mach-O) | prebound-undefined (Mach-O) | indirect (Mach-O) | object (ELF)| function (ELF) | file   (ELF) | common (ELF) | tls (ELF)) String
    - `section`: �������ֶβ�Ϊ�յĻ���������������
    - `section.id` String containing section index, segment name (if applicable) and section name �C same format as r2��s section IDs
    - `section.protection` ��д���ԣ�����"rwx"
    - `section.name` �������� String
    - `section.address` ���Ե�ַ NativePointer
    - `section.size` if present, a Number specifying the symbol��s size in bytes
- `Module.enumerateSymbolsSync(name)` Module.enumerateSymbols() ��ͬ���汾
  - `Module.enumerateRanges(name, protection, callbacks)` ���ܻ�����ͬ�� Process.enumerateRanges(), ֻ��������һ��ģ�����޶���ö�ٵķ�Χ
  - `Module.findExportByName(exportName)` ���ݵ����������ƻ�ȡ������ַ����������ڶ�Ӧ�ĵ�������������null
  - `Module.load(path)` ����ָ��·����ģ��, �������ʧ�ܣ������쳣
  - `Module.ensureInitialized(name)` ȷ��ָ��ģ��ĳ�ʼ�������Ѿ����С��������ڼ���ڼ����Ҫ, �������ڽ����������ڵ��������У��Ա��ܹ���ȫ���� API����
  - `Module.findBaseAddress(name)`��ȡģ�����ַ, �����ȡʧ�ܣ������쳣
  - `Module.findExportByName(moduleName|null, exportName)`
  - `Module.getExportByName(moduleName|null, exportName)` ��ָ��ģ���м��ص������������ģ������δ֪��������null�����Ứ��ʱ��ȥ��������ģ�飬ȥ���Ҷ�Ӧ�ĵ�������
### ModuleMap
  `new ModuleMap([filter])` �������Ϊ�ڴ�ģ����գ���ҪĿ���ǿ�����Ϊһ��ģ���ٲ�������������������������ٶ�λһ������ĵ�ַ�������ĸ�ģ�顣����ModuleMap��ʱ�򣬾��Ƕ�Ŀ����̵�ǰ���ص�ģ�����Ϣ��һ�����գ�������Ҫ�������������Ϣ��ʱ�򣬿���ʹ�� update ���и��¡� ��� filter �����ǿ�ѡ�ģ���Ҫ��������������ĵ�ģ�飬����������С���յķ�Χ��ע�⣺filter�ǹ��˺����������ַ�����������Ϊ����ģ����������������˺����ķ���ֵҪ����Ϊtrue����֮��Ϊfalse����������ڴ��е�ģ�������Ϣ�����ˣ� ��������������filter������
  - `has(address)` ����ַ�Ƿ���ģ����
  - `find(address), get(address)` ���� address ��ַ��ָ���ģ�������ϸ��Ϣ����������� find ����null��get ֱ�ӻ��׳��쳣������ķ��صĶ������ϸ��Ϣ�����Բο� Process.enumerateModules()
  - `findName(address), getName(address), findPath(address), getPath(address)` ��find(address)��get(address)�����������ƣ���������ģ�����ƻ���·���ֶ�
  - `update()` ����ModuleMap��Ϣ�������ģ����ػ���ж�أ���õ���һ�Σ����ʹ�þ�����
  - `values()` ����map�е�����ģ�����, ���(���ٴε���update��ֵ�����ᷢ���ı�)
### Memory
  - `Memory.scan(address, size, pattern, callbacks)` ��address�ڴ濪ʼ����СΪsizeָ�����ڴ��в��ҷ���patternģʽ��ֵ��pattern��ʽ"13 37 ?? ff" ���߸��߼������뷽ʽ��"13 37 13 37 : 1f ff ff f1"��ƥ���ֵΪ�����ֵ&ƥ���ֵ������ָ�����ֽڵ�ͨ���"?3 37 13 ?7"
    - `onMatch(address, size)` ��ƥ�䵽��Ӧ��ֵNativePointer and Number
    - `onError(reason)` �������ڴ���ʳ����쳣ʱ��ֹͣ��reason�᷵�ش��� String
    - `onComplete()` ��ָ��ɨ���ڴ�ɨ�����ʱ
  - `Memory.scanSync(address, size, pattern)` ͬ��ɨ�跽ʽan array of objects
    - address: ���Ե�ַ NativePointer.
    - size: ��С
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
  - Memory.alloc(size[, options]): �ڶѿռ�������ָ����С���ڴ�, �������Ĵ�СΪ����ҳ��Ĵ�С�������Ҫ�����ڴ�ĵ�ַ�ӽ������ĵ�ַ����ͨ�� { near: address, maxDistance: distanceInBytes }ѡ������ȡ�ӽ�ָ����ַ���ڴ�
  - Memory.copy(dst, src, n): ��c����memcpy����
  - Memory.dup(address, size): ��c����dup����, �������ڴ棬�ڽ��п���
  - Memory.protect(address, size, protection): �����ڴ�ҳ������� boolean
```javascript
  Memory.protect(ptr('0x1234'), 4096, 'rw-');
```
  - `Memory.patchCode(address, size, apply)` ��ȫ�򲹶������޸�ָ��address���ڴ��е�ֵ
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
  - `MemoryAccessMonitor.enable(ranges, callbacks)` ���һ�������ڴ��ķ��ʣ��ڴ������ڴ���ʵ�ʱ�򷢳�֪ͨ��rangesҪô��һ���������ڴ�飬Ҫô��һ���ڴ�����飬ÿ���ڴ������������ԣ�
      - `ranges.base` �����ڴ����ַ
      - `ranges.size` �����ڴ�Ĵ�С
    callbacks����Ľṹ
      - `onAccess: function(details)` �������ʵ�ʱ��ͬ�������������
        - `details.operation` �����ڴ���ʵĲ������ͣ�ȡֵ��Χ��(read, write, or execute)
        - `details.from` �����ڴ���ʵ�ָ���ַ
        - `details.address` �����ʵ��ڴ��ַ
        - `details.rangeIndex` �����ʵ��ڴ������������ǵ���MemoryAccessMonitor.enable()��ʱ��ָ�����ڴ�����
        - `details.pageIndex` �ڱ�����ڴ�鷶Χ�ڵ�ҳ�����
        - `details.pagesCompleted` ��ĿǰΪֹ�Ѿ��������ڴ���ʵ�ҳ��ĸ������Ѿ��������ڴ���ʵ�ҳ�潫���ٽ��м�أ�
        - `details.pagesTotal` ��ʼָ������Ҫ��ص��ڴ�ҳ������
  - `MemoryAccessMonitor.disable()` ֹͣ���ҳ����ʲ���
### CModule
new CModule(code[, symbols, options]): �����ṩ�Ĵ���һ���µ�Cģ��, ��ģ������ǰ����������CԴ������ַ����� Ҳ������Ԥ����Ĺ�����ArrayBuffer��Cģ�鱻ӳ�䵽�ڴ��У����ҿ��Ա�js������ȫ���ʣ�ȫ�ֺ������ᱻ�Զ�����ΪNativePointer todo
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
new ApiResolver(type): �����������͵��½�����todo����������������
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
  - `DebugSymbol.fromAddress(address), DebugSymbol.fromName(name)` ��ָ����ַ����ָ�����Ʋ��ҷ�����Ϣ�����صķ�����Ϣ���������������ԣ�
    - address: ��ǰ���ŵĵ�ַ.
    - name: ��ǰ���ŵ�����, δ֪����null
    - moduleName: �������ڵ�ģ������, δ֪����null
    - fileName:  �������ڵ��ļ���, δ֪����null
    - lineNumber: �������ڵ��ļ��ڵ��к�, δ֪����null
```
  const f = Module.getExportByName('libcommonCrypto.dylib',
      'CCCryptorCreate');
  Interceptor.attach(f, {
    onEnter(args) {
      //javascript Array map��������һ�������飬�����е�Ԫ��Ϊԭʼ����Ԫ�ص��ú���������ֵ��
      //javascript Array join���� �����������Ԫ�ط���һ���ַ�����
      console.log('CCCryptorCreate called from:\n' +
        Thread.backtrace(this.context, Backtracer.ACCURATE)
        .map(DebugSymbol.fromAddress).join('\n') + '\n');
    }
  });
```
  - `DebugSymbol.getFunctionByName(name)` ���ݷ�����Ϣ�����Һ������������ NativePointer ����
  - `DebugSymbol.findFunctionsNamed(name)` ���ݷ�����Ϣ�����Һ������������ NativePointer ����
  - `DebugSymbol.findFunctionsMatching(glob)` ���ݷ�����Ϣ�����Һ������������ NativePointer ����
  - `DebugSymbol.load(path)` ����ָ��ģ��ķ�����Ϣ
### Kernel��cy��
Kernel.available��һ������ֵ��ָ���ں�API�Ƿ���á��������������������Ҫ�����κ������ں����Ի򷽷���

Kernel.base: �ں˵Ļ�ַ����UInt64��

Kernel.pageSize: �ں�ҳ�Ĵ�С�����ֽ�Ϊ��λ�������ֱ�ʾ

Kernel.enumerateModules(): ö�ٵ�ǰ���ص��ں�ģ�飬���ذ����������ԵĶ������飺

?	name:��Ϊ�ַ����Ĺ淶ģ����
?	base: ������ַ��Ϊ����ָ��
?	size: ��С���ֽڣ�

Kernel.enumerateRanges(protection|specifier): ö�����㱣��Ҫ����ں��ڴ淶Χ����ʽΪ��rwx������rw-��ʾ���������ٿɶ�д�������ߣ�������Ϊ˵���������ṩһ��ֵ���������ı����������ϣ���ϲ�������ͬ���������ڷ�Χ������Խ��ϲ�������Ϊtrue��Ĭ��ֵΪfalse��i�� e.���ַ�Χ�ֿ��������ذ����������ԵĶ������飺

?	base: ��Ϊ����ָ��Ļ�ַ
?	size: ��С���ֽڣ�
?	protection: �������������ģ�

Kernel.enumerateModuleRanges(name, protection):����Kernel.enumerateRangesһ����ֻ����������������ָ����ģ�������������ں˱����ģ�飬�����ƿ���Ϊ�ա�ÿ����Χ����һ�������ֶΣ����а���һ����Ϊ�ַ�����Ψһ��ʶ����

Kernel.alloc(size): �����ں��ڴ�Ĵ�С�ֽڣ��������뵽�ں�ҳ���С�ı���������ֵ��һ��UInt64��ָ������Ļ�ַ

Kernel.protect(address, size, protection): �����ں��ڴ�����ı��������б�������kernel.enumerateRanges������ʽ��ͬ���ַ�����

����:

Kernel.protect(UInt64('0x1234'), 4096, 'rw-');
	Kernel.readByteArray(address, length): just like NativePointer#readByteArray, but reading from kernel memory.

?	Kernel.writeByteArray(address, bytes): just like NativePointer#writeByteArray, but writing to kernel memory.

?	Kernel.scan(address, size, pattern, callbacks): just like Memory.scan, but scanning kernel memory.

?	Kernel.scanSync(address, size, pattern): synchronous version of scan() that returns the matches in an array.

## Data Types, Function and Callback

### Int64
- `new Int64(v)`: ��vΪ����������һ��Int64����v������һ����ֵ��Ҳ������һ���ַ�����ʽ����ֵ��ʾ��Ҳ����ʹ�� Int64(v) ���ּ򵥵ķ�ʽ��
- `add(rhs)`, `sub(rhs)`, `and(rhs)`, `or(rhs)`, `xor(rhs)` Int64��صļӼ��˳�����������
- `shr(n)`, `shl(n)` Int64��ص����ơ����Ʋ���
- `compare(rhs)` Int64�ıȽϲ������е�����String.localCompare()
- `toNumber()` ��Int64ת����һ��ʵ��
- `toString([radix = 10])` ����һ������ֵ���ư�Int64ת���ַ�����Ĭ����ʮ����

### UInt64
- `new UInt64(v)`
- `add(rhs)`, `sub(rhs)`, `and(rhs)`, `or(rhs)`, `xor(rhs)`
- `shr(n)`, `shl(n)`
- `compare(rhs)`
- `toNumber()`
- `toString([radix = 10])`

### NativePointer
- `new NativePointer(s)`: ���ַ���sΪ����������һ��NativePointer����Ҳ����ʹ�� ptr(s) ���ּ򵥵ķ�ʽ��
- `isNull()` ����һ������ֵ������������ؼ��ָ���Ƿ�Ϊ��
- `add(rhs)`, `sub(rhs)`, `and(rhs)`, `or(rhs)`, `xor(rhs)`, `xor`rhs`
- `shr(n)`, `shl(n)`
- `not()`
- `sign([key, data])` todo
- `strip([key])` todo
- `blend(smallInteger)` todo
- `equals(rhs)`:����һ������ֵ��ָʾ��rhs���Ƿ���ڴ�ֵ��i�� ������ͬ��ָ��ֵ
- `compare(rhs)`: ���������ȽϽ�� just like **String#localeCompare()**
- `toInt32()`: casts this **NativePointer** to a signed 32-bit integer
- `toString([radix = 16])`:ת��Ϊ��ѡ�������ַ�����Ĭ��Ϊ16��
- `toMatchPattern()`: ���ذ���[`Memory.scan����`]���ַ���(https://frida.re/docs/javascript-api/#memory-��ָ���ԭʼֵ�ļ���ƥ��ģʽ
- `readPointer()`:��ȡ[`NativePointer`](https://frida.re/docs/javascript-api/#nativepointer)���������λ�á�
  �����ַ���ɶ���������JavaScript�쳣��
- `writePointer(ptr)`: ����ptr��д����ڴ�λ�á�
  �����ַ����д��������JavaScript�쳣��
- `readS8()`, `readU8()`, `readS16()`, `readU16()`, `readS32()`, `readU32()`, `readShort()`, `readUShort()`, `readInt()`, `readUInt()`, `readFloat()`, `readDouble()`: �Ӹ��ڴ�λ�ö�ȡ�з��Ż��޷���8/16/32/etc.��float/doubleֵ����������Ϊ���ַ��ء�
  �����ַ���ɶ���������JavaScript�쳣��

- `writeS8(value)`, `writeU8(value)`, `writeS16(value)`, `writeU16(value)`, `writeS32(value)`, `writeU32(value)`, `writeShort(value)`, `writeUShort(value)`, `writeInt(value)`, `writeUInt(value)`, `writeFloat(value)`, `writeDouble(value)`: ���з��Ż��޷���8/16/32/etc.��float/double��value��д����ڴ�λ�á�
  �����ַ����д��������JavaScript�쳣��

- `readS64()`, `readU64()`, `readLong()`, `readULong()`:�Ӹ��ڴ�λ�ö�ȡ�з��Ż��޷��ŵ�64λ���򳤴�С��ֵ����������Ϊ**Int64**/**UInt64**ֵ���ء�
  �����ַ���ɶ���������JavaScript�쳣��

- `writeS64(value)`, `writeU64(value)`, `writeLong(value)`, `writeULong(value)`: ��**Int64**/**UInt64**`value`д����ڴ�λ�á�
  �����ַ����д��������JavaScript�쳣��

- `readByteArray(length)`: re�Ӹ��ڴ�λ�ö�ȡ`length`�ֽڣ���������Ϊ**ArrayBuffer**���ء�ͨ������������Ϊ�ڶ����������ݵ�[`send����`]������Ч�ش��䵽����Frida��Ӧ�ó���(https://frida.re/docs/javascript-api/#communication-���ͣ���
  ����ӵ�ַ��ȡ���κΡ����ȡ��ֽڲ��ɶ���������JavaScript�쳣��

- `writeByteArray(bytes)`: ����bytes��д����ڴ�λ�ã����С�bytes����һ��**ArrayBuffer**��ͨ���ӡ�readByteArray����`�����أ�������һ��0��255֮����������顣`

   `. For example: `[ 0x13, 0x37, 0x42 ]`.

  ���д���ַ���κ��ֽڶ�����д���������JavaScript�쳣��

- `readCString([size = -1])`, `readUtf8String([size = -1])`, `readUtf16String([length = -1])`, `readAnsiString([size = -1])`: ��ASCII��UTF-8��UTF-16��ANSI�ַ�������ʽ��ȡ���ڴ�λ�õ��ֽڡ����֪���ַ����Ĵ�С�����ֽ�Ϊ��λ�������ṩ��ѡ�ġ�size������������ַ�����NUL��β������Ըò�����ָ��*-1*��ͬ���������֪���ַ����ĳ��ȣ����ַ�Ϊ��λ����������ṩ��ѡ��'length'������
  ����ӵ�ַ��ȡ���κΡ�size��/��length���ֽڲ��ɶ���������JavaScript�쳣��

  ��ע�⣬`readAnsiString����`����Windows�Ͽ��ã�����أ���

  `writeUtf8String(str)`, `writeUtf16String(str)`, `writeAnsiString(str)`:-��JavaScript�ַ������벢д����ڴ�λ�ã�ʹ��NUL��ֹ������
  ���д���ַ���κ��ֽڶ�����д���������JavaScript�쳣��
  ע�⣬`WriteAsisting����`����Windows�Ͽ��ã�����أ���


### ArrayBuffer

- `wrap(address, size)`: �����������ڴ�����֧�ֵ�ArrayBuffer������address��ָ���������ַ��NativePointer��size��ָ�����С�����֡���NativePointer��/дapi��ͬ��access��ִ���κ���֤������ζ�Ż�ָ�뽫ʹ���̱�����
- `unwrap()`: ����һ��NativePointer��ָ��ArrayBuffer�ı��ݴ洢�Ļ�ַ�����÷��������ں󱸴洢����ʹ��ʱ���ֻ������Ļ״̬��

- `new NativeFunction(address, returnType, argTypes[, abi])`:����һ���µ�NativeFunction�����á�address�����ĺ�������[`NativePointer`]ָ����(https://frida.re/docs/javascript-api/#nativepointer))�����С�returnType��ָ���������ͣ���argTypes������ָ���������͡��������ϵͳĬ��ֵ��Ҳ����ѡ��ָ����abi�������ڿɱ亯�����ڹ̶������Ϳɱ����֮��ġ�argTypes�������һ����������Ŀ��

### NativeFunction
- `new NativeFunction(address, returnType, argTypes[, abi])`: ��address��ʹ��NativePointer�ĸ�ʽ����ַ�ϴ���һ��NativeFunction���������к������ã�returnTypeָ�������������ͣ�argTypesָ�������Ĳ������ͣ��������ϵͳĬ�����ͣ�Ҳ����ѡ���Ե�ָ��abi���������ڿɱ����͵ĺ������ڹ̶�����֮��ʹ�� ������ ����ʾ��
- ��ͽṹ��
    �ں������õĹ����У���ͽṹ���ǰ�ֵ���ݵģ����ݵķ�ʽ��ʹ��һ���������ֱ�ָ����ͽṹ��ĸ����ֶΣ�������Ϊ�˺���Ҫ�������Ӧ��������������ǿ���֧������Ƕ�׵ģ��ṹ����๹�����֮��ʹ��NativePointer����ʽ���صģ����Ҳ���Դ��ݸ�Interceptor.attach() ���á���Ҫע��ĵ��ǣ� ���ݵ�����һ��Ҫ����Ҫ�Ĳ����ṹ���ϸ��Ǻϣ�����һ�������Ĳ�����һ��3�����εĽṹ�壬�ǲ������ݵ�ʱ��һ��Ҫ�� [��int��, ��int��, ��int��]������һ��ӵ���麯��������˵�����õ�ʱ�򣬵�һ������һ�������ָ�롣
- ֧������
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
- ֧�ֵĵ���Լ��
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
`new NativeFunction(address, returnType, argTypes[, options])`:��ǰ��Ĺ��캯��һ���������ĸ�����options��һ�����󣬿��ܰ�������һ����������
  - `option.abi`
  - `option.scheduling` cooperative: ���������߳��ڵ��ñ�������ʱִ��JavaScript���룬���ڵ���ǰ�ͷ�����Ȼ�����»�ȡ������Ĭ����Ϊ��exclusive: �ڵ��ñ�������ʱ�����������߳�ִ��JavaScript���룬������JavaScript���������죬�����ܻᵼ��������
  - `option.exceptions` steal: ��������õĺ��������쳣�����磬ͨ��ȡ������Чָ������ã���Frida���ͷŶ�ջ����ȡ�쳣������ת��Ϊ���Դ����JavaScript�쳣������ܻ�ʹӦ�ó�����δ�����״̬���������ڱ���������ʱʹ���̱���������Ĭ����Ϊ��propagate: ��Ӧ�ó������������ڼ䷢�����κα����쳣(���ߣ�ͨ��Process.setExceptionHandler������װ�Ĵ�����򡣣�
  - `option.traps` default: ����������ô����κι��ӣ�������Interceptor.attach�����ص���all: �����������ص���������Ҳ������ÿ�κ��������ڼ���ʱ���¼������ڲ������븲���ʡ�ָ��fuzzer���ڵ�������ʵ�֡�����ִ�С��ȷǳ����á���ע�⣬��ʹ��Java��ObjC APIʱ����Ҳ�ǿ��ܵģ���Ϊ������װ�����ṩ��¡��options��API�����������Զ���NativeFunctionѡ����·�����װ����

### NativeCallback
- `new NativeCallback(func, returnType, argTypes[, abi])`: ʹ��JavaScript���� func ������һ��Native����������returnType��argTypes�ֱ�ָ�������ķ������ͺͲ����������顣�������ʹ��ϵͳĬ�ϵ� abi ���ͣ������ָ�� abi �������������argTypes��abi���ͣ����Բ鿴NativeFunction���˽���ϸ��Ϣ���������ķ�������Ҳ��NativePointer���ͣ���˿�����Ϊ Interceptor.replace �Ĳ���ʹ�á�
### SystemFunction
- `new SystemFunction(address, returnType, argTypes[, abi])` ���ܻ�����NativeFunctionһ�£�����ʹ�����������Ի�ȡ�������̵߳�last error״̬������ֵ�Ƕ�ƽ̨��ص���ֵ��һ���װ��Ϊvalue���󣬱����Ƕ�������ֵ�ķ�װ�� errno(UNIX) ���� lastError(Windows)��
- `new SystemFunction(address, returnType, argTypes[, options])`: ͬ�ϣ�����option������NativeFunction���캯�����ơ�

## Network

### Socket

- `Socket.listen([options])`: ��TCP��UNIX�����׽��֡����ؽ���SocketListener�ľ����
  Ĭ��Ϊͬʱ����IPv4��IPv6�����֧�֣����������ѡ���TCP�˿��ϵ����нӿڡ�
  ��ѡѡ�������һ�����ܰ�������ĳЩ���Ķ���
  - `family` Э���ַ��(unix | ipv4 | ipv6)
  - `host`: ������ַ
  - `port`: �����˿�
  - `type`: unixЭ����socket����(anonymous | path | abstract | abstract-padded)
  - `path`: UNIX socket path as a string.
  - `backlog`: ���������� number. Defaults to `10`.
- `Socket.connect(options)`: ����TCP Server����Unix Server. Returns a *Promise* that receives a **SocketConnection**.

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

���з���������ȫ�첽�ģ�������Promise����

- `path`: (UNIX family) path being listened on.
- `port`: (IP family) IP port being listened on.
- `close()`:�ر����������ͷ�������ص���Դ��һ���������رգ�����������������ʧ�ܡ������ιر������������Ҳ��ᵼ�´���
- `accept()`: �ȴ���һ���ͻ������ӡ����صĳ�ŵ����SocketConnection��

### SocketConnection

�̳�IOStream�����з���������ȫ�첽�ģ�������Promise����



- `setNoDelay(noDelay)`: ���noDelayΪtrue�������Nagle�㷨��������������Nagle�㷨��Ĭ������������õģ����ֻ����ϣ���Ż����ӳٶ����Ǹ�������ʱ����Ҫ���ô˷�����

------

## File and Stream
### File
- `new File(filePath, mode)` ��filePath���򿪻򴴽��ļ���ģʽ�ַ���ָ����δ򿪸��ļ������硰wb���Զ�����ģʽ���ļ�����д��(��C��׼���е�fopen()��ʽ��ͬ)
- `write(data)`: ͬ���ؽ�����д���ļ������е����ݿ������ַ�����Ҳ��������NativePointer#readByteArray���صĻ�����
- `flush()`: f�����л�������ˢ�µ������ļ�
- `close()`: �ر��ļ�����Ӧ���ڴ������ļ�����ô˺������������Զ��������ռ���ű���ж��ʱ���������û�������
```javascript
var file = new File("/sdcard/reg.dat", "r+");
file.write("EoPAoY62@ElRD");
file.flush();
file.close();
```

### IOStream
���з���������ȫ�첽�ģ�������Promise����
- `input`: the **InputStream** to read from.
- `output`: the **OutputStream** to write to.
- `close()`: �ر������ͷ���֮��ص���Դ����Ҳ���رյ���������������һ�����رգ�����������������ʧ�ܡ������ιر��������ᵼ�´���

### InputStream

���з���������ȫ�첽�ģ�������Promise����

- `close()`: �ر������ͷ���֮��ص���Դ��һ�����رգ�����������������ʧ�ܡ������ιر��������Ҳ��ᵼ�´���
- `read(size)`: �����ж�ȡ����С�ֽڡ����صĳ�ŵ����һ�������ֽڵ�ArrayBuffer������ĩβͨ��һ���ջ����������źš�
- `readAll(size)`: ���������ж�ȡ��ֱ��������ȷ�еĴ�С�ֽڡ����صĳ�ŵ����һ��ArrayBuffer�����ĳ����������ֽڡ������������Ľ����ᵼ�³�ŵ���ܾ��������ִ������д��������а������������ݵ�partialData���ԡ�

### OutputStream

���з���������ȫ�첽�ģ�������Promise����
	`close()`: �ر������ͷ�������ص���Դ��һ�����رգ�����������������ʧ�ܡ������ιر��������Ҳ��ᵼ�´���

- `write(data)`: ���Խ�����д����������ֵ������ArrayBuffer��Ҳ������0��255֮����������顣���ص�Promise����һ�����֣�ָ��������д���˶����ֽڵ����ݡ�
- `writeAll(data)`: һֱд������ֱ���������ݶ���д�롣����ֵ��ArrayBuffer�����0��255֮����������顣�������������������´������д���������partialSize���ԣ�ָ���ڴ�����֮ǰ����д���˶����ֽڵ����ݡ�
- `writeMemoryRegion(address, size)`: ���Խ���С�ֽ�д�������ӵ�ַ��NativePointer����ȡ���ǡ����ص�Promise����һ�����֣�������ָ��д�����������ֽ�����

### UnixInputStream

(Only available on UNIX-like OSes.)



- `new UnixInputStream(fd[, options])`: ��ָ�����ļ�������fd�����µ�InputStream��
  ���������ṩһ��options���󣬽�autoClose����Ϊtrue���Ա����ͷ���ʱ��ͨ��close�����������������գ�ʹ���رյײ��ļ���������

### UnixOutputStream

(Only available on UNIX-like OSes.)



- `new UnixOutputStream(fd[, options])`: ��ָ�����ļ�������fd�����µ�OutputStream��
  ���������ṩһ��options���󣬽�autoClose����Ϊtrue���Ա����ͷ���ʱ��ͨ��close�����������������գ�ʹ���رյײ��ļ���������

### Win32InputStream

(Only available on Windows.)



- `new Win32InputStream(handle[, options])`: ��ָ���ľ����Windows���ֵ�������µ�InputStream��
  ���������ṩһ��options���󣬽�autoClose����Ϊtrue���Ա����ͷ���ʱ��ͨ��close�����������������գ�ʹ���رյײ�����

### Win32OutputStream

(Only available on Windows.)



- `new Win32OutputStream(handle[, options])`: ��ָ���ľ����Windows���ֵ�������µ�OutputStream��
  ���������ṩһ��options���󣬽�autoClose����Ϊtrue���Ա����ͷ���ʱ��ͨ��close�����������������գ�ʹ���رյײ�����


#### ���ݿ�
### Sqlite���ݿ�
- SqliteDatabase.open(path[, options]): ���� ָ���� SQLite v3 ���ݿ�path��һ���������ݿ��ļ�ϵͳ·�����ַ�����Ĭ������£����ݿ⽫�Զ�д��ʽ�򿪣���������ͨ���ṩ����options��Ϊ �����ԵĶ���flagsָ����������һ������ֵ���ַ����������Զ������Ϊ��readonly��readwrite��create�����ص� SqliteDatabase ���������������ݿ�ִ�в�ѯ��

- SqliteDatabase.openInline(encodedContents): ����open()�����ݿ��������Ϊ���������ݵ��ַ����ṩ��Base64 ���롣���ǽ����ڶ����ݿ���� Base64 ����֮ǰ�����ݿ���� gzip�������ǿ�ѡ�ģ����ҿ���ͨ������ gzip ħ���������⡣���ݿ��Զ�д��ʽ�򿪣��� 100% ���ڴ��У��Ӳ��Ӵ��ļ�ϵͳ���������Ҫ����Ԥ�������ݻ���Ĵ�������ã���������ָ����̬�����ľ�̬�������ݡ�

- close(): �ر����ݿ⡣������ݿ��������Ӧ�õ��ô˺����������ڶ��������ռ���ű�ж��ʱ�������������

- exec(sql)��ִ��ԭʼ SQL ��ѯ������sql�ǰ�����ѯ�ı���ʾ���ַ�������ѯ�Ľ���������ԣ������Ӧ�ý������������ݿ�Ĳ�ѯ�����������

- prepare(sql): ���ṩ�� SQL ����� SqliteStatement��������sql�ǰ�����ѯ�ı���ʾ���ַ�����
���磺
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

- dump(): �����ݿ�ת��������Ϊ Base64 �� gzip ѹ�� blob�����н����Ϊ�ַ������ء�������ڴ��������������������ã�ͨ������SqliteDatabase.openInline().


### Sqlite���
- bindInteger(index, value): ��������value��index
- bindFloat(index, value): �������value��index
- bindText(index, value): ���ı���value��index
- bindBlob(index, bytes): �� blob ��bytes��index������bytes ��ArrayBuffer���ֽ�ֵ������ַ���
- bindNull(index): ��һ����ֵ�󶨵� index
- step(): Ҫô��ʼһ���µĲ�ѯ����õ�һ�������Ҫô�ƶ�����һ��������һ�����飬���а�������ѯָ����˳���null�ڴﵽ���һ�����ʱ��ֵ��reset()����������ٴ�ʹ�ô˶�����Ӧ�ڴ�ʱ���� ��
- reset(): �����ڲ�״̬�����������ѯ

#### �Ǳ�
### ������
- `Interceptor.attach(target, callbacks[, data])`:  ��targetָ����λ�ý��к����������أ�target��һ��NativePointer����������ָ������Ҫ���صĺ����ĵ�ַ����һ����Ҫע�⣬��32λARM�����ϣ�ARM������ַĩλһ����0��2�ֽڶ��룩��Thumb������ַĩλһ��1�����ֽڶ��룩�����ʹ�õĺ�����ַ����Frida API��ȡ�Ļ��� ��ôAPI�ڲ����Զ��������ϸ�ڣ����磺Module.findExportByName()������ʵ��Frida�����ڴ��������λ�ý������أ���������һ�� callbacks �ص���ʱ����Ϊ�ص�λ���п��ܲ��ں����Ŀ�ͷ������onEnter�����Ļص�����Fridaֻ�ܾ����ı�֤���������ص�λ��ǰ��Ĵ���û���޸Ĺ�����Ĳ��������������ں���ͷ��������ȷ����ȷ����������attach���÷���һ���������󣬺���ȡ�����ص�ʱ�򣬿�����Ϊ Interceptor.detach() �Ĳ���ʹ�ã�����һ���ȽϷ���ĵط����Ǿ����ڻص��������棬������һ�����ص� this ���߳�tls���󣬷����ڻص������д洢��������������� onEnter �б���ֵ��Ȼ���� onLeave ��ʹ�á����⣬this�����л����� returnAddress: ����NativePointer���͵� address ����, context: ���� pc��sp���Լ���ؼĴ������� eax, ebx�ȣ������ڻص�������ֱ���޸�, errno: UNIX����ǰ�̵߳Ĵ���ֵ. lastError: (Windows) ��ǰ�̵߳Ĵ���ֵ, threadId: ����ϵͳ�߳� ID, depth: �������ò�����. ����ע������ṩ�Ļص����������ش�Ӱ�졣�����ֻ��Ҫ�������������ķ���ֵ������ֻ���ķ���ֵ����ϵ��������ȷ��ʡ�Բ���Ҫ�Ļص��������⽫�����߼�����onEnter�в���onLeave��Ϊ�ջص����������iPhone 5S�ϣ����ṩonEnterʱ�Ļ������������� 6 ΢�룬�����ṩonEnter ��onLeave ʱ��Ϊ11 ΢�롣��ҪС�����ض�ÿ����������εĺ����ĵ��ã���Ȼsend()���첽�ģ������͵�����Ϣ���ܿ�����δ��Ը�Ƶ�����Ż������ǣ����ҹ��Ⱥ���ʱ�������Խ��Interceptor ��CModule��ʹ�������� C ��ʵ�ֻص�������callbacks������һ�����󣬴��½ṹ���£�
  - `onEnter: function(args)`: �����غ�������֮ǰ�ص�������ԭʼ�����Ĳ���ʹ��args���飨NativePointer�������飩����ʾ�������������޸ĺ����ĵ��ò�����
  - `onLeave: function(retval)`�������غ�������֮��ص�������retval��ʾԭʼ�����ķ���ֵ��retval�Ǵ�NativePointer�̳����ģ��Ƕ�ԭʼ����ֵ��һ����װ�������ʹ��retval.replace(1337)�������޸ķ���ֵ�����ݡ���Ҫע���һ���ǣ�retval����ֻ�� onLeave����������Χ����Ч����������Ҫ������������Ա�����ʹ�õĻ���һ��Ҫʹ�������������󣬱��磺ptr(retval.toString())��
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
- `Interceptor.detachAll()`: ȡ��֮ǰ���е����ص���
- `Interceptor.replace(target, replacement[, data])`: ����ʵ�ִ����滻�����������Ҫ������Ҫ��ȫ�滻��һ��ԭ�к�����ʵ�ֵ�ʱ����ʹ�ã�ע��replacement����ʹ��JavaScript��ʽ��һ��NativeCallback��ʵ�֣����������Ҫȡ������滻Ч��������ʹ�� Interceptor.revert������ʵ�֣�����㻹�������Լ����滻���������������ԭʼ�ĺ���������ʹ���� target Ϊ������NativeFunction����������
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
- `Interceptor.revert(target)` ��ԭ������ԭʼʵ���߼�����ȡ��ǰ��� Interceptor.replace����
- `Interceptor.flush()` ȷ��֮ǰ���ڴ��޸Ĳ�����ִ����ϣ������Ѿ����ڴ��з������ã�ֻҪ�������������Ҫ������ã��������ִ���� attach() ���� replace() ���ã�Ȼ�������Ҫʹ��NativeFunction����Ժ������е��ã������������Ҫ����flush����������£�����ĵ��ò������ڵ�ǰ�̼߳����뿪JavaScript����ʱ�������ߵ���send()��ʱ���Զ�����flush������Ҳ������Щ�ײ����� send() �����ĺ��������� RPC �����������κε�console API

#### Stalker
- `Stalker.exclude(range)`: ��ָ�����ڴ���rangeΪ�ų��ģ�����һ������base��size���ԵĶ��󡪡������� eg ���صĶ����е�����Process.getModuleByName()��

����ζ�ŵ������Դ��෶Χ�ڵ�ָ��ĵ���ʱ��Stalker ���������ִ�С���ˣ������ܹ��۲�/�޸Ĵ���Ĳ����ͷ��صķ���ֵ�������ῴ������������֮���ָ�

������������ܺͽ���������

- `Stalker.follow([threadId, options])` ��ʼ�����߳�IDΪ threadId������Ǳ��̣߳�����ʡ�ԣ����߳��¼�
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

����ע������
�ṩ�Ļص����������ش�Ӱ�졣�����ֻ��Ҫ���ڵ���ժҪ��������ԭʼ�¼��������෴����ȷ��ʡ�Բ���Ҫ�Ļص��������⽫�����߼�����onCallSummary �в��� onReceive��Ϊ�ջص��������

����ע�⣬Stalker ������CModule���ʹ�ã�����ζ�Żص������� C ��ʵ�֡�

- `Stalker.unfollow([threadId])` ֹͣ����߳��¼�������ǵ�ǰ�̣߳������ʡ�� threadId ����
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
- `Stalker.flush()`: ����κλ�����¼�����������ȵ���һ��Stalker.queueDrainInterval�δ���ʱ�����á�
- `Stalker.garbageCollect()`: �ڵ���Stalker.unfollow()֮����һ�����ʵ�ʱ���ͷŶ�Ӧ���ڴ棬���Ա�����߳̾�̬�����µ��ڴ��ͷ����⡣
- `Stalker.invalidate(address)`: ʹ����������ĵ�ǰ�̵߳ķ��������Ч�����ṩת���ص���ϣ��Ϊ�����Ļ����鶯̬�������ʱ�����á����ȡ����ע�����¹�ע�߳�Ҫ��Ч�ö࣬���߻ᶪ�����л���ķ��벢Ҫ���ͷ��ʼ�������������Ļ����顣

- `Stalker.invalidate(threadId, address)`: ʹ������������ض��̵߳ķ��������Ч�����ṩת���ص���ϣ��Ϊ�����Ļ����鶯̬�������ʱ�����á����ȡ����ע�����¹�ע�߳�Ҫ��Ч�ö࣬���߻ᶪ�����л���ķ��벢Ҫ���ͷ��ʼ�������������Ļ����顣

- `Stalker.addCallProbe(address, callback[, data])` ��address��ַ���ĺ��������õ�ʱ�򣬵���callback���󣨶������ͺ�Interceptor.attach.onEnterһ�£�������һ��Id�����Ը������Stalker.removeCallProbeʹ��
  - `void onCall (GumCallSite * site, gpointer user_data)`
����������£���������ѡ����data������ a��NativePointer ��ֵ��Ϊ ���ݸ��ص�user_data��

- `Stalker.removeCallProbe`: ɾ���� Stalker#addCallProbe.

- `Stalker.trustThreshold`: һ��������ָ��һ�δ�����Ҫִ�ж��ٴβ��ܱ���Ϊ�������������ᷢ�����졣ָ�� -1 ��ʾ�����Σ�������0 ��ʾ��һ��ʼ�����δ��룬N ��ʾ��ִ�� N �κ����δ��롣Ĭ��Ϊ 1��

- `Stalker.queueCapacity` ָ���¼����еĳ��ȣ�Ĭ�ϳ�����16384
- `Stalker.queueDrainInterval` �¼����в�ѯ�ɷ�ʱ������Ĭ����250ms��Ҳ����˵1�����¼����л���ѯ4��

#### ������
- `WeakRef.bind(value, fn)`��һ���������ռ���ű�������ж�أ��ͼ���value������fn�ص�value������һ�����Դ��ݸ�WeakRef.unbind() ��ʽ����� id ��

��������ڹ������԰󶨣���������Ҫ JS ֵʱ��Ҫ�ͷű�����Դ����� API �����á�

- `WeakRef.unbind(id)`: ֹͣ���Ӵ��ݸ� ��ֵ WeakRef.bind(value, fn)����fn�������ûص���

#### Java
- `Java.available` һ������ֵ����ʾ��ǰ�������Ƿ�����������õ�Java�����������Dalvik����Art��������ʹ��Java����֮ǰ��ʹ�����������ȷ������������
- `Java.androidVersion` һ���ַ�����ָ�������������е� Android �汾��
- `Java.enumerateLoadedClasses(callbacks)` ö�ٵ�ǰ�������Ѿ����ص��࣬ÿ��ö�ٵ����ص���ص�callbacks:
  - `onMatch(name, handle)` ö�ٵ�һ���࣬�������ƽ��лص�����������ƺ���������Ϊ Java.use() �Ĳ�������ȡ�����һ�����ö���
  - `onComplete()`: ��ö����������ʱ���á�
- `Java.enumerateLoadedClassesSync()`: enumerateLoadedClasses()ͬ���汾 
- `Java.enumerateClassLoaders(callbacks)`: ö�� Java VM �д��ڵ��������������callbacks��ָ���Ķ���
  - `onMatch(loader)`: Ϊÿ������������ã�loader�ض��İ�װ��java.lang.ClassLoader��
  - `onComplete()`: ��ö�������������ʱ���á�
�����Խ������ļ���������Java.ClassFactory.get()�� .use()ָ������������ϵ��ࡣ
- `Java.enumerateClassLoadersSync()`: ͬ���汾 enumerateClassLoaders()���������е����������
- `Java.enumerateMethods(query)`: ö��ƥ��ķ���query��ָ��Ϊ"class!method"������ʹ�� globs��Ҳ���Ժ�׺/��һ���������η���
  - `i`: �����ִ�Сд��ƥ�䡣
  - `s`: ��������ǩ�������� eg"putInt"��� "putInt(java.lang. String, int): void".
  - `u`: ���û�������࣬����ϵͳ�ࡣ
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
      ��
    ]
  }
]
```
- `Java.scheduleOnMainThread(fn)` ����������߳���ִ�к���fn
- `Java.perform(fn)`: ȷ����ǰ�̸߳��ӵ� VM ������fn. ���������� Java �Ļص��в��Ǳ���ġ���fn���Ӧ�ó������������в����ã����Ƴٵ��á�Java.performNow()�������Ҫ����Ӧ�ó�����࣬��ʹ�á�
```javascript
Java.perform(() => {
  const Activity = Java.use('android.app.Activity');
  Activity.onResume.implementation = function () {
    send('onResume() got called! Let\'s call the original implementation');
    this.onResume();
  };
});
```
- `Java.performNow(fn)`: ȷ����ǰ�̸߳��ӵ� VM ������fn. ���������� Java �Ļص��в��Ǳ���ġ���
- `Java.use(className)`: ��̬��ȡһ�� JavaScript ��װ�� className��������ͨ������$new()�������ù��캯����ʵ�������󡣵���$dispose()һ��ʵ������ʽ������������ȴ� JavaScript ���������ռ�����ű���ж�أ�����̬�ͷǾ�̬���������ã������������滻����ʵ�ֲ������׳��쳣��
```javascript
Java.perform(() => {
  const Activity = Java.use('android.app.Activity');
  const Exception = Java.use('java.lang.Exception');
  Activity.onResume.implementation = function () {
    throw Exception.$new('Oh noes!');
  };
});
```
Ĭ�������ʹ��Ӧ�ó���������������������ͨ������ͬ�ļ�����ʵ�������Java.classFactory.loader.

��ע�⣬���з�����װ�����ṩ��һ��clone(options)API ������������ ����NativeFunctionѡ����·�����װ����

- `Java.openClassFile(filePath)`: �� .dex �ļ�filePath��ʹ�����·�������һ������

  - `oad()`: ������������ص� VM �С�
  
  - `etClassNames()`: ��ȡ�������������顣

- `Java.choose(className, callbacks)` ��Java���ڴ����ɨ��ָ�������Ƶ�Java����ÿ��ɨ�赽һ��������ص�callbacks:
  - `onMatch(instance)` ÿ��ɨ�赽һ��ʵ�����󣬵���һ�Σ���������stop����ɨ��Ĺ���
  - `onComplete()` �����еĶ���ɨ�����֮����лص�
- `Java.retain(obj)`: ���� JavaScript ��װ��obj���Ժ����滻����֮��ʹ�á�
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

- `Java.cast(handle, klass)` ʹ�ö�����handle����klass��Java.use�������أ������ʹ���һ�������JavaScript���ã�����������ð���һ��class��������ȡ��ǰ������࣬Ҳ����һ��$className��������ȡ�������ַ��������磺
```javascript
const Activity = Java.use('android.app.Activity');
const activity = Java.cast(ptr('0x1234'), Activity);
```

- `Java.array(type, elements)`:type�� JavaScript ���鴴������ָ��Ԫ�ص�Java ����elements�����ɵ� Java �������Ϊ������ JS ���飬������ͨ�����ô��ݸ� Java API �����������޸������ݡ�
```java
const values = Java.array('int', [ 1003, 1005, 1007 ]);

const JString = Java.use('java.lang.String');
const str = JString.$new(Java.array('byte', [ 0x48, 0x65, 0x69 ]));
```

- `Java.isMainThread()`: �жϵ������Ƿ������߳������С�

- `Java.registerClass(spec)`������һ���µ� Java �ಢΪ�䷵��һ����װ��������spec��һ�������������ݵĶ���

  - `name`: ָ���������ַ�����
  - `superClass`������ѡ��������ʡ�Լ̳��� java.lang.Object.
  - `implements`������ѡ������ʵ�ֵĽӿ����顣
  - `fields`������ѡ��ָ��Ҫ������ÿ���ֶε����ƺ����͵Ķ���
  - `methods`������ѡ������ָ��Ҫʵ�ֵķ�����

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

- `Java.deoptimizeEverything()`: ǿ�� VM ʹ���������ִ�����в�������ĳЩ������б�Ҫ��ֹ�Ż��ƹ������ҹ���������ʹ�� ART �� Instrumentation API ����������ʱ��
- `Java.deoptimizeBootImage()`: ������ Java.deoptimizeEverything() ��ֻ������ӳ��������ȥ�Ż���ʹ���� dalvik.vm.dex2oat-flags --inline-max-code-units=0������Ч����
- `Java.vm`���������·����Ķ���
  - `perform(fn)`: ȷ����ǰ�̸߳��ӵ� VM ������fn. ���������� Java �Ļص��в��Ǳ���ġ���
  - `getEnv()`: ��ȡ��ǰ�̵߳�JNIEnv. �����ǰ�߳�δ���ӵ� VM���������쳣��
  - `tryGetEnv()`: ����Ϊ��ǰ�̵߳�JNIEnv. ����null�����ǰ�߳�û�����ӵ��������
- `Java.classFactory`������ʵ�������Ĭ���๤�� Java.use()��ʹ��Ӧ�ó���������������
- `Java.ClassFactory`: �����������Ե��ࣺ
  - `get(classLoader)`: ��ȡ��������������๤��ʵ����Ļ��ʹ�õ�Ĭ���๤������Ӧ�ó�����������������������Java.enumerateClassLoaders()ͨ���� API�������������������֮������
  - `loader`: ֻ�����ԣ�Ϊ��ǰʹ�õ���������ṩ��װ��������Ĭ�ϵ��๤������ͨ����һ�ε���������Java.perform()��
  - `cacheDir`: ������ǰʹ�õĻ���Ŀ¼·�����ַ���������Ĭ�ϵ��๤������ͨ����һ�ε���������Java.perform()��
  - `tempFileNaming`: ָ��������ʱ�ļ�������Լ���Ķ���Ĭ��Ϊ{ prefix: 'frida', suffix: 'dat' }.
  - `use(className)`: ����Java.use()�������ض������������
  - `openClassFile(filePath)`: ����Java.openClassFile() �������ض������������
  - `choose(className, callbacks)`: ����Java.choose()�������ض������������
  - `retain(obj)`: ����Java.retain()�������ض������������
  - `cast(handle, klass)`: ����Java.cast()�������ض������������
  - `array(type, elements)`: ����Java.array()�������ض������������
  - `registerClass(spec)`: ����Java.registerClass()�������ض������������
### CPUָ��
- `Instruction.parse(target)` �� target ָ���ĵ�ַ������ָ�����target��һ��NativePointer��ע�⣬��32λARM�ϣ�ARM������ַ��Ҫ��2�ֽڶ���ģ�Thumb������ַ��1�ֽڶ���ģ��������ʹ��Frida����ĺ�������ȡ��target��ַ��Frida���Զ���������ϸ�ڣ�����operands��groups��ϸ�ڣ���ο�CapStone�ĵ�. parse�������صĶ��������������
  - address����ǰָ���EIP��NativePointer����
  - next������ָ��ĵ�ַ�����Լ���ʹ��parse����
  - size: ��ǰָ���С
  - mnemonic: ָ�����Ƿ�
  - opStr: �ַ�����ʽ��ʾ������
  - operands�����������飬ÿ���������������type��value�������ԣ�����ƽ̨��ͬ���п��ܻ�����һЩ��������
  - regsRead: ����ָ����ʽ���ж�ȡ�ļĴ�������
  - regsWritten: ����ָ����ʽ��д��ļĴ�������
  - groups: ����ָ��������ָ�����
  - toString(): ��ָ���ʽ����һ���˱Ƚ����׶������ַ�����ʽ
  
#### X86Writer
- `new X86Writer(codeAddress[, { pc: ptr('0x1234') }])`������һ���µĴ����д������������ֱ��д���ڴ�� x86 �������� codeAddress��ָ��ΪNativePointer���ڶ���������һ����ѡ��ѡ��������п���ָ����ʼ��������������ڽ��������ɵ��ݴ滺����ʱ�����á���Memory.patchCode() �� iOS��ʹ��ʱ�Ǳز����ٵģ������ܻ�Ϊ���ṩһ����ʱλ�ã���λ���Ժ��ӳ�䵽Ԥ���ڴ�λ�õ��ڴ��С�
- `reset(codeAddress[, { pc: ptr('0x1234') }])`: ����ʵ��
- `dispose()`: ���е������ڴ�
- `flush()`��������ǩ���ò�������������д���ڴ档һ��������ɴ��룬��Ӧ��ʼ�յ�������ͨ��Ҳϣ���ڲ���صĴ����֮��ִ�д˲�����������һ�������ɶ������ʱ��
- `base`: �����һ���ֽڵ��ڴ�λ�ã���ΪNativePointer
- `code`: �������һ���ֽڵ��ڴ�λ�ã���ΪNativePointer
- `pc`: ���������һ���ֽڴ��ĳ������������ΪNativePointer
- `offset`: ��ǰƫ������Ϊ JavaScript ����
- `putLabel(id)`: �ڵ�ǰλ�÷�һ����ǩ������id��һ���ַ������ڹ�ȥ��δ����put*Label()�����п��ܻᱻ����
- `putCallAddressWithArguments(func, args)`: put ���ô���ָ���� C ��������Ĵ���args��ָ��Ϊ JavaScript ���飬����ÿ��Ԫ��Ҫô��ָ���Ĵ������ַ�����Ҫô��ָ������ֵ�� Number �� NativePointer��
- `putCallAddressWithAlignedArguments(func, args)`: ������һ������Ҳȷ�������б��� 16 �ֽڱ߽��϶���
- `putCallRegWithArguments(reg, args)`: put ���ô���ָ���� C ��������Ĵ���args��ָ��Ϊ JavaScript ���飬����ÿ��Ԫ��Ҫô��ָ���Ĵ������ַ�����Ҫô��ָ������ֵ�� Number �� NativePointer��
- `putCallRegWithAlignedArguments(reg, args)`: ������һ������Ҳȷ�������б��� 16 �ֽڱ߽��϶���
- `putCallRegOffsetPtrWithArguments(reg, offset, args)`: put ���ô���ָ���� C ��������Ĵ���args��ָ��Ϊ JavaScript ���飬����ÿ��Ԫ��Ҫô��ָ���Ĵ������ַ�����Ҫô��ָ������ֵ�� Number �� NativePointer��
- `putCallAddress(address)`: ����һ�� CALL ָ��
- `putCallReg(reg)`: ����һ�� CALL ָ��
- `putCallRegOffsetPtr(reg, offset)`: ����һ�� CALL ָ��
- `putCallIndirect(addr)`: ����һ�� CALL ָ�
- `putCallIndirectLabel(labelId)`: ����һ�� CALL ָ������labelId���ɹ�ȥ��δ������putLabel()
- `putCallNearLabel(labelId)`: ����һ�� CALL ָ������labelId���ɹ�ȥ��δ������putLabel()
- `putLeave()`: ��һ�� LEAVE ָ��
- `putRet()`: ��һ��RETָ��
- `putRetImm(immValue)`: ��һ��RETָ��
- `putJmpAddress(address)`: ��һ��JMPָ��
- `putJmpShortLabel(labelId)`: ����һ�� JMP ָ������labelId���ɹ�ȥ��δ������putLabel()
- `putJmpNearLabel(labelId)`: ����һ�� JMP ָ������labelId���ɹ�ȥ��δ������putLabel()
- `putJmpReg(reg)`: ��һ��JMPָ��
- `putJmpRegPtr(reg)`: ��һ��JMPָ��
- `putJmpRegOffsetPtr(reg, offset)`: ��һ��JMPָ��
- `putJmpNearPtr(address)`: ��һ��JMPָ��
- `putJccShort(instructionId, target, hint)`: ��һ��JCCָ��
- `putJccNear(instructionId, target, hint)`: ��һ��JCCָ��
- `putJccShortLabel(instructionId, labelId, hint)`: ����һ�� JCC ָ������labelId���ɹ�ȥ��δ������putLabel()
- `putJccNearLabel(instructionId, labelId, hint)`: ����һ�� JCC ָ������labelId���ɹ�ȥ��δ������putLabel()
- `putAddRegImm(reg, immValue)`: ��һ�� ADD ָ��
- `putAddRegReg(dstReg, srcReg)`: ��һ�� ADD ָ��
- `putAddRegNearPtr(dstReg, srcAddress)`: ��һ�� ADD ָ��
- `putSubRegImm(reg, immValue)`: ����һ�� SUB ָ��
- `putSubRegReg(dstReg, srcReg)`: ����һ�� SUB ָ��
- `putSubRegNearPtr(dstReg, srcAddress)`: ����һ�� SUB ָ��
- `putIncReg(reg)`: ����һ��INCָ��
- `putDecReg(reg)`: ��һ�� DEC ָ��
- `putIncRegPtr(target, reg)`: ����һ��INCָ��
- `putDecRegPtr(target, reg)`: ��һ�� DEC ָ��
- putLockXaddRegPtrReg(dstReg, srcReg): ��һ��LOCK XADDָ��

- putLockCmpxchgRegPtrReg(dstReg, srcReg): ��һ��LOCK CMPXCHG ָ��

- putLockIncImm32Ptr(target): ��һ��LOCK INC IMM32ָ��

- putLockDecImm32Ptr(target): ��һ��LOCK DEC IMM32ָ��

- putAndRegReg(dstReg, srcReg): ����һ�� AND ָ��

- putAndRegU32(reg, immValue): ����һ�� AND ָ��

- putShlRegU8(reg, immValue): ��һ��SHLָ��

- putShrRegU8(reg, immValue): ��һ��SHRָ��

- putXorRegReg(dstReg, srcReg): ����һ�� XOR ָ��

- putMovRegReg(dstReg, srcReg): ��һ�� MOV ָ��

- putMovRegU32(dstReg, immValue): ��һ�� MOV ָ��

- putMovRegU64(dstReg, immValue): ��һ�� MOV ָ��

- putMovRegAddress(dstReg, address): ��һ�� MOV ָ��

- putMovRegPtrU32(dstReg, immValue): ��һ�� MOV ָ��

- putMovRegOffsetPtrU32(dstReg, dstOffset, immValue): ��һ�� MOV ָ��

- putMovRegPtrReg(dstReg, srcReg): ��һ�� MOV ָ��

- putMovRegOffsetPtrReg(dstReg, dstOffset, srcReg): ��һ�� MOV ָ��

- putMovRegRegPtr(dstReg, srcReg): ��һ�� MOV ָ��

- putMovRegRegOffsetPtr(dstReg, srcReg, srcOffset): ��һ�� MOV ָ��

- putMovRegBaseIndexScaleOffsetPtr(dstReg, baseReg, indexReg, scale, offset): ��һ�� MOV ָ��

- putMovRegNearPtr(dstReg, srcAddress): ��һ�� MOV ָ��

- putMovNearPtrReg(dstAddress, srcReg): ��һ�� MOV ָ��

- putMovFsU32PtrReg(fsOffset, srcReg): ����һ�� MOV FS ָ��

- putMovRegFsU32Ptr(dstReg, fsOffset): ����һ�� MOV FS ָ��

- putMovGsU32PtrReg(fsOffset, srcReg): ����һ�� MOV GS ָ��

- putMovRegGsU32Ptr(dstReg, fsOffset): ����һ�� MOV GS ָ��

- putMovqXmm0EspOffsetPtr(offset): ��һ�� MOVQ XMM0 ESP ָ��

- putMovqEaxOffsetPtrXmm0(offset): ����һ�� MOVQ EAX XMM0 ָ��

- putMovdquXmm0EspOffsetPtr(offset): ��һ�� MOVDQU XMM0 ESP ָ��

- putMovdquEaxOffsetPtrXmm0(offset): ����һ�� MOVDQU EAX XMM0 ָ��

- putLeaRegRegOffset(dstReg, srcReg, srcOffset): ��һ�� LEA ָ��

- putXchgRegRegPtr(leftReg, rightReg): ��һ��XCHGָ��

- putPushU32(immValue): ����һ��PUSHָ��

- putPushNearPtr(address): ����һ��PUSHָ��

- putPushReg(reg): ����һ��PUSHָ��

- putPopReg(reg): ��һ��POPָ��

- putPushImmPtr(immPtr): ����һ��PUSHָ��

- putPushax(): ����һ�� PUSHAX ָ��

- putPopax(): ����һ�� POPAX ָ��

- putPushfx(): ����һ�� PUSHFX ָ��

- putPopfx(): ��һ�� POPFX ָ��

- putTestRegReg(regA, regB): ��һ�� TEST ָ��

- putTestRegU32(reg, immValue): ��һ�� TEST ָ��

- putCmpRegI32(reg, immValue): ��һ��CMPָ��

- utCmpRegOffsetPtrReg(regA, offset, regB): ��һ��CMPָ��

- putCmpImmPtrImmU32(immPtr, immValue): ��һ��CMPָ��

- putCmpRegReg(regA, regB): ��һ��CMPָ��

- putClc(): ��һ�� CLC ָ��

- putStc(): ��һ��STCָ��

- putCld(): ��һ��CLDָ��

- putStd(): ��һ�� STD ָ��

- putCpuid(): ��һ��CPUIDָ��

- putLfence(): ����һ�� LFENCE ָ��

- putRdtsc(): ��һ�� RDTSC ָ��

- putPause(): ��һ�� PAUSE ָ��

- putNop(): ��һ�� NOP ָ��

- putBreakpoint(): ����һ������ϵͳ/�ܹ��ض��Ķϵ�ָ��

- putPadding(n): ��n����ָ��

- putNopPadding(n): ��nNOP ָ��

- putU8(value): ��һ�� uint8

- putS8(value): ��һ�� int8

- putBytes(data): ���ṩ��ArrayBuffer �з���ԭʼ����

#### X86�ض�λ��
- `new X86Relocator(inputCode, output)`������һ���µĴ����ض�λ�������ڽ� x86 ָ���һ���ڴ�λ�ø��Ƶ���һ���ڴ�λ�ã�ע����Ӧ�ص�����λ����ص�ָ�Դ��ַ��inputCodeһ��NativePointerָ����Ŀ���� ����output��һ��X86Writerָ�������Ŀ���ڴ��ַ��

- `reset(inputCode, output)`: ����ʵ��

- `dispose()`: ���е������ڴ�

- `input`:��ĿǰΪֹ�Ķ�������ָ�null ��ÿ�ε��� ʱ��ʼ������readOne()��

- `eob`: boolean ָʾ�Ƿ��ѵ����β���������ѵ����κ����͵ķ�֧���� CALL��JMP��BL��RET��

- `eoi`������ֵ��ָʾ�Ƿ��ѵ���������������磬�����ѵ��� JMP/B/RET��һ��ָ��֮�������Ҳ����û����Ч���롣

- `readOne()`: ����һ��ָ������ض�λ�����ڲ��������������ص�ĿǰΪֹ��ȡ���ֽ���������֮ǰ�ĵ��á������Լ������ô˷����Ա��ֻ��壬������������writeOne()��skipOne()�����ߣ������Ի��嵽����ĵ㣬Ȼ�����writeAll(). �����������ʱ�����㣬����ζ�Ÿ�eoi������ now true��

- `peekNextWriteInsn()`: ͵����һ��Ҫд���������ָ��

- `peekNextWriteSource()`: �鿴��һ��Ҫд���������ָ��ĵ�ַ

- `skipOne()`: ����������Ҫд��ָ��

- `skipOneNoLabel()`: ������������д��ָ���û���ڲ�ʹ�õı�ǩ��������˽���֧�ض�λ���ض�λ��Χ�ڵ�λ�ã������Ƕ����з�֧������д���������Ż������� Frida ��Stalker����

- `writeOne()`: д��һ������ָ��

- `writeOneNoLabel()`: д��һ������ָ���û���ڲ�ʹ�õı�ǩ��������˽���֧�ض�λ���ض�λ��Χ�ڵ�λ�ã������Ƕ����з�֧������д���������Ż������� Frida ��Stalker����

- `writeAll()`: д�����л���ָ��

#### x86 ö������
- �Ǽǣ� xax xcx xdx xbx xsp xbp xsi xdi eax ecx edx ebx esp ebp esi edi rax rcx rdx rbx rsp rbp rsi rdi r8 r9 r10 r11 r12 r13 r14 r15 r8d r9d r10d r11d r12d r13d r14d r15d xip eip rip
- ָ�� ID�� jo jno jb jae je jne jbe ja js jns jp jnp jl jge jle jg jcxz jecxz jrcxz
- ��֧��ʾ�� no-hint likely unlikely
- ָ��Ŀ�꣺ byte dword qword

#### д��
- new ArmWriter(codeAddress[, { pc: ptr('0x1234') }])������һ���µĴ����д������������ֱ��д���ڴ�� ARM �������� codeAddress��ָ��ΪNativePointer���ڶ���������һ����ѡ��ѡ��������п���ָ����ʼ��������������ڽ��������ɵ��ݴ滺����ʱ�����á���Memory.patchCode() �� iOS��ʹ��ʱ�Ǳز����ٵģ������ܻ�Ϊ���ṩһ����ʱλ�ã���λ���Ժ��ӳ�䵽Ԥ���ڴ�λ�õ��ڴ��С�

- reset(codeAddress[, { pc: ptr('0x1234') }]): ����ʵ��

- dispose(): ���е������ڴ�

- flush()��������ǩ���ò�������������д���ڴ档һ��������ɴ��룬��Ӧ��ʼ�յ�������ͨ��Ҳϣ���ڲ���صĴ����֮��ִ�д˲�����������һ�������ɶ������ʱ��

- base: �����һ���ֽڵ��ڴ�λ�ã���ΪNativePointer

- code: �������һ���ֽڵ��ڴ�λ�ã���ΪNativePointer

- pc: ���������һ���ֽڴ��ĳ������������ΪNativePointer

- offset: ��ǰƫ������Ϊ JavaScript ����

- skip(nBytes)�� ���� nBytes

- putLabel(id): �ڵ�ǰλ�÷�һ����ǩ������id��һ���ַ������ڹ�ȥ��δ����put*Label()�����п��ܻᱻ����

- putCallAddressWithArguments(func, args): put ���ô���ָ���� C ��������Ĵ���args��ָ��Ϊ JavaScript ���飬����ÿ��Ԫ��Ҫô��ָ���Ĵ������ַ�����Ҫô��ָ������ֵ�� Number �� NativePointer��

- putBranchAddress(address): ���÷�֧/��ת��������ַ����Ĵ���

- canBranchDirectlyBetween(from, to): ȷ���������������ڴ�λ��֮���Ƿ���ܴ���ֱ�ӷ�֧

- putBImm(target): ��һ��Bָ��

- putBCondImm(cc, target): ����һ�� B COND ָ��

- putBLabel(labelId): ����һ�� B ָ������labelId���ɹ�ȥ��δ������putLabel()

- putBCondLabel(cc, labelId): ����һ�� B COND ָ������labelId���ɹ�ȥ��δ������putLabel()

- putBlImm(target): ��һ��BLָ��

- putBlxImm(target): ��һ��BLXָ��

- putBlLabel(labelId): ����һ��BLָ������labelId���ɹ�ȥ��δ������putLabel()

- putBxReg(reg): ��һ��BXָ��

- putBlxReg(reg): ��һ��BLXָ��

- putRet(): ��һ��RETָ��

- putLdrRegAddress(reg, address): ��һ�� LDR ָ��

- putLdrRegU32(reg, val): ��һ�� LDR ָ��

- putLdrRegRegOffset(dstReg, srcReg, srcOffset): ��һ�� LDR ָ��

- putLdrCondRegRegOffset(cc, dstReg, srcReg, srcOffset): ����һ�� LDR COND ָ��

- putLdmiaRegMask(reg, mask): ����һ�� LDMIA MASK ָ��

- putStrRegRegOffset(srcReg, dstReg, dstOffset): ��һ��STRָ��

- putStrCondRegRegOffset(cc, srcReg, dstReg, dstOffset): ����һ�� STR COND ָ��

- putMovRegReg(dstReg, srcReg): ��һ�� MOV ָ��

- putMovRegRegShift(dstReg, srcReg, shift, shiftValue): ����һ�� MOV SHIFT ָ��

- putMovRegCpsr(reg): ����һ�� MOV CPSR ָ��

- putMovCpsrReg(reg): ����һ�� MOV CPSR ָ��

- putAddRegU16(dstReg, val): ��һ�� ADD U16 ָ��

- putAddRegU32(dstReg, val): ��һ�� ADD ָ��

- putAddRegRegImm(dstReg, srcReg, immVal): ��һ�� ADD ָ��

- putAddRegRegReg(dstReg, srcReg1, srcReg2): ��һ�� ADD ָ��

- putAddRegRegRegShift(dstReg, srcReg1, srcReg2, shift, shiftValue): ����һ�� ADD SHIFT ָ��

- putSubRegU16(dstReg, val): ����һ�� SUB U16 ָ��

- putSubRegU32(dstReg, val): ����һ�� SUB ָ��

- putSubRegRegImm(dstReg, srcReg, immVal): ����һ�� SUB ָ��

- putSubRegRegReg(dstReg, srcReg1, srcReg2): ����һ�� SUB ָ��

- putAndsRegRegImm(dstReg, srcReg, immVal): ����һ�� ANDS ָ��

- putCmpRegImm(dstReg, immVal): ��һ��CMPָ��

- putNop(): ��һ�� NOP ָ��

- putBreakpoint(): ����һ������ϵͳ/�ܹ��ض��Ķϵ�ָ��

- putBrkImm(imm): ��һ�� BRK ָ��

- putInstruction(insn): ��ԭʼָ����Ϊ JavaScript ���

- putBytes(data): ���ṩ��ArrayBuffer �з���ԭʼ����

#### ���ض�λ��
- `new ArmRelocator(inputCode, output)`������һ���µĴ����ض�λ�������ڽ� ARM ָ���һ���ڴ�λ�ø��Ƶ���һ���ڴ�λ�ã�ע����Ӧ�ص���λ����ص�ָ�Դ��ַ��inputCodeһ��NativePointerָ����Ŀ����output��һ��ָ������Ŀ���ڴ��ַ��ArmWriter������

- `reset(inputCode, output)`: ����ʵ��

- `dispose()`: ���е������ڴ�

- `input`:��ĿǰΪֹ�Ķ�������ָ�null ��ÿ�ε��� ʱ��ʼ������readOne()��

- `eob`: boolean ָʾ�Ƿ��ѵ����β���������ѵ����κ����͵ķ�֧���� CALL��JMP��BL��RET��

- `eoi`������ֵ��ָʾ�Ƿ��ѵ���������������磬�����ѵ��� JMP/B/RET��һ��ָ��֮�������Ҳ����û����Ч���롣

- `readOne()`: ����һ��ָ������ض�λ�����ڲ��������������ص�ĿǰΪֹ��ȡ���ֽ���������֮ǰ�ĵ��á������Լ������ô˷����Ա��ֻ��壬������������writeOne()��skipOne()�����ߣ������Ի��嵽����ĵ㣬Ȼ�����writeAll(). �����������ʱ�����㣬����ζ�Ÿ�eoi������ now true��

- `peekNextWriteInsn()`: ͵����һ��Ҫд���������ָ��

- `peekNextWriteSource()`: �鿴��һ��Ҫд���������ָ��ĵ�ַ

- `skipOne()`: ����������Ҫд��ָ��

- `writeOne()`: д��һ������ָ��

- `writeAll()`: д�����л���ָ��

#### Ĵָ����
- new ThumbWriter(codeAddress[, { pc: ptr('0x1234') }])������һ���µĴ����д������������ֱ��д���ڴ�� ARM �������� codeAddress��ָ��ΪNativePointer���ڶ���������һ����ѡ��ѡ��������п���ָ����ʼ��������������ڽ��������ɵ��ݴ滺����ʱ�����á���Memory.patchCode() �� iOS��ʹ��ʱ�Ǳز����ٵģ������ܻ�Ϊ���ṩһ����ʱλ�ã���λ���Ժ��ӳ�䵽Ԥ���ڴ�λ�õ��ڴ��С�

- reset(codeAddress[, { pc: ptr('0x1234') }]): ����ʵ��

- dispose(): ���е������ڴ�

- flush()��������ǩ���ò�������������д���ڴ档һ��������ɴ��룬��Ӧ��ʼ�յ�������ͨ��Ҳϣ���ڲ���صĴ����֮��ִ�д˲�����������һ�������ɶ������ʱ��

- base: �����һ���ֽڵ��ڴ�λ�ã���ΪNativePointer

- code: �������һ���ֽڵ��ڴ�λ�ã���ΪNativePointer

- pc: ���������һ���ֽڴ��ĳ������������ΪNativePointer

- offset: ��ǰƫ������Ϊ JavaScript ����

- skip(nBytes)�� ���� nBytes

- putLabel(id): �ڵ�ǰλ�÷�һ����ǩ������id��һ���ַ������ڹ�ȥ��δ����put*Label()�����п��ܻᱻ����

- commitLabel(id): �ύ�Ը�����ǩ�ĵ�һ�����������ã�true�ɹ����ء�����false��������ı�ǩ��δ������أ���������û�и���Ĵ������á�

- putCallAddressWithArguments(func, args): put ���ô���ָ���� C ��������Ĵ���args��ָ��Ϊ JavaScript ���飬����ÿ��Ԫ��Ҫô��ָ���Ĵ������ַ�����Ҫô��ָ������ֵ�� Number �� NativePointer��

- putCallRegWithArguments(reg, args): put ���ô���ָ���� C ��������Ĵ���args��ָ��Ϊ JavaScript ���飬����ÿ��Ԫ��Ҫô��ָ���Ĵ������ַ�����Ҫô��ָ������ֵ�� Number �� NativePointer��

- putBImm(target): ��һ��Bָ��

- putBLabel(labelId): ����һ�� B ָ������labelId���ɹ�ȥ��δ������putLabel()

- putBLabelWide(labelId): ��һ�� B WIDE ָ��

- putBxReg(reg): ��һ��BXָ��

- putBlImm(target): ��һ��BLָ��

- putBlLabel(labelId): ����һ��BLָ������labelId���ɹ�ȥ��δ������putLabel()

- putBlxImm(target): ��һ��BLXָ��

- putBlxReg(reg): ��һ��BLXָ��

- putCmpRegImm(reg, immValue): ��һ��CMPָ��

- putBeqLabel(labelId): ����һ�� BEQ ָ������labelId���ɹ�ȥ��δ������putLabel()

- putBneLabel(labelId): ����һ�� BNE ָ������labelId���ɹ�ȥ��δ������putLabel()

- putBCondLabel(cc, labelId): ����һ�� B COND ָ������labelId���ɹ�ȥ��δ������putLabel()

- putBCondLabelWide(cc, labelId): ����һ�� B COND WIDE ָ��

- putCbzRegLabel(reg, labelId): ����һ�� CBZ ָ������labelId���ɹ�ȥ��δ������putLabel()

- putCbnzRegLabel(reg, labelId): ����һ�� CBNZ ָ������labelId���ɹ�ȥ��δ������putLabel()

- putPushRegs(regs): ʹ��ָ���ļĴ������� PUSH ָ�ָ��Ϊ JavaScript ���飬����ÿ��Ԫ�ض���ָ���Ĵ������Ƶ��ַ�����

- putPopRegs(regs): ʹ��ָ���ļĴ������� POP ָ�ָ��Ϊ JavaScript ���飬����ÿ��Ԫ�ض���ָ���Ĵ������Ƶ��ַ�����

- putLdrRegAddress(reg, address): ��һ�� LDR ָ��

- putLdrRegU32(reg, val): ��һ�� LDR ָ��

- putLdrRegReg(dstReg, srcReg): ��һ�� LDR ָ��

- putLdrRegRegOffset(dstReg, srcReg, srcOffset): ��һ�� LDR ָ��

- putLdrbRegReg(dstReg, srcReg): ��һ��LDRBָ��

- putVldrRegRegOffset(dstReg, srcReg, srcOffset): ��һ��VLDRָ��

- putLdmiaRegMask(reg, mask): ����һ�� LDMIA MASK ָ��

- putStrRegReg(srcReg, dstReg): ��һ��STRָ��

- putStrRegRegOffset(srcReg, dstReg, dstOffset): ��һ��STRָ��

- putMovRegReg(dstReg, srcReg): ��һ�� MOV ָ��

- putMovRegU8(dstReg, immValue): ��һ�� MOV ָ��

- putMovRegCpsr(reg): ����һ�� MOV CPSR ָ��

- putMovCpsrReg(reg): ����һ�� MOV CPSR ָ��

- putAddRegImm(dstReg, immValue): ��һ�� ADD ָ��

putAddRegReg(dstReg, srcReg): ��һ�� ADD ָ��

putAddRegRegReg(dstReg, leftReg, rightReg): ��һ�� ADD ָ��

putAddRegRegImm(dstReg, leftReg, rightValue): ��һ�� ADD ָ��

putSubRegImm(dstReg, immValue): ����һ�� SUB ָ��

putSubRegReg(dstReg, srcReg): ����һ�� SUB ָ��

putSubRegRegReg(dstReg, leftReg, rightReg): ����һ�� SUB ָ��

putSubRegRegImm(dstReg, leftReg, rightValue): ����һ�� SUB ָ��

putAndRegRegImm(dstReg, leftReg, rightValue): ����һ�� AND ָ��

putLslsRegRegImm(dstReg, leftReg, rightValue): ��һ�� LSLS ָ��

putLsrsRegRegImm(dstReg, leftReg, rightValue): ��һ�� LSRS ָ��

putMrsRegReg(dstReg, srcReg): ��һ�� MRS ָ��

putMsrRegReg(dstReg, srcReg): ��һ�� MSR ָ��

putNop(): ��һ�� NOP ָ��

- putBkptImm(imm): ��һ��BKPTָ��

- putBreakpoint(): ����һ������ϵͳ/�ܹ��ض��Ķϵ�ָ��

- putInstruction(insn): ��ԭʼָ����Ϊ JavaScript ���

- putInstructionWide(upper, lower): ������ JavaScript Number ֵ�з���һ��ԭʼ�� Thumb-2 ָ��

- putBytes(data): ���ṩ��ArrayBuffer �з���ԭʼ����

#### Ĵָ�ض�λ��
- new ThumbRelocator(inputCode, output)������һ���µĴ����ض�λ�������ڽ� ARM ָ���һ���ڴ�λ�ø��Ƶ���һ���ڴ�λ�ã�ע����Ӧ�ص���λ����ص�ָ�Դ��ַ��inputCodeһ��NativePointerָ����Ŀ����output��һ��ָ������Ŀ���ڴ��ַ��ThumbWriter������

- reset(inputCode, output): ����ʵ��

- dispose(): ���е������ڴ�

- input:��ĿǰΪֹ�Ķ�������ָ�null ��ÿ�ε��� ʱ��ʼ������readOne()��

- eob: boolean ָʾ�Ƿ��ѵ����β���������ѵ����κ����͵ķ�֧���� CALL��JMP��BL��RET��

- eoi������ֵ��ָʾ�Ƿ��ѵ���������������磬�����ѵ��� JMP/B/RET��һ��ָ��֮�������Ҳ����û����Ч���롣

- readOne(): ����һ��ָ������ض�λ�����ڲ��������������ص�ĿǰΪֹ��ȡ���ֽ���������֮ǰ�ĵ��á������Լ������ô˷����Ա��ֻ��壬������������writeOne()��skipOne()�����ߣ������Ի��嵽����ĵ㣬Ȼ�����writeAll(). �����������ʱ�����㣬����ζ�Ÿ�eoi������ now true��

- peekNextWriteInsn(): ͵����һ��Ҫд���������ָ��

- peekNextWriteSource(): �鿴��һ��Ҫд���������ָ��ĵ�ַ

- skipOne(): ����������Ҫд��ָ��

- writeOne(): д��һ������ָ��

- copyOne(): ������һ������ָ�����ǰ�������꣬������д��ͬһ��ָ��

- writeAll(): д�����л���ָ��

#### ARM ö������
- �Ǽǣ� r0 r1 r2 r3 r4 r5 r6 r7 r8 r9 r10 r11 r12 r13 r14 r15 sp lr sb sl fp ip pc
- ϵͳע�᣺ apsr-nzcvq
- �������룺 eq ne hs lo mi pl vs vc hi ls ge lt gt le al
- ���ٸˣ� asr lsl lsr ror rrx asr-reg lsl-reg lsr-reg ror-reg rrx-reg

#### Arm64Writer
- `new Arm64Writer(codeAddress[, { pc: ptr('0x1234') }])`������һ���µĴ����д������������ֱ��д���ڴ�� AArch64 �������� codeAddress��ָ��ΪNativePointer���ڶ���������һ����ѡ��ѡ��������п���ָ����ʼ��������������ڽ��������ɵ��ݴ滺����ʱ�����á���Memory.patchCode() �� iOS��ʹ��ʱ�Ǳز����ٵģ������ܻ�Ϊ���ṩһ����ʱλ�ã���λ���Ժ��ӳ�䵽Ԥ���ڴ�λ�õ��ڴ��С�

- `reset(codeAddress[, { pc: ptr('0x1234') }])`: ����ʵ��

- `dispose()`: ���е������ڴ�

- flush()��������ǩ���ò�������������д���ڴ档һ��������ɴ��룬��Ӧ��ʼ�յ�������ͨ��Ҳϣ���ڲ���صĴ����֮��ִ�д˲�����������һ�������ɶ������ʱ��

- base: �����һ���ֽڵ��ڴ�λ�ã���Ϊ NativePointer

- code: �������һ���ֽڵ��ڴ�λ�ã���Ϊ NativePointer

- pc: ���������һ���ֽڴ��ĳ������������Ϊ NativePointer

- offset: ��ǰƫ������Ϊ JavaScript ����

- skip(nBytes)�� ���� nBytes

- putLabel(id): �ڵ�ǰλ�÷�һ����ǩ������id��һ���ַ������ڹ�ȥ��δ����put*Label()�����п��ܻᱻ����

- putCallAddressWithArguments(func, args): put ���ô���ָ���� C ��������Ĵ���args��ָ��Ϊ JavaScript ���飬����ÿ��Ԫ��Ҫô��ָ���Ĵ������ַ�����Ҫô��ָ������ֵ�� Number �� NativePointer��

- putCallRegWithArguments(reg, args): put ���ô���ָ���� C ��������Ĵ���args��ָ��Ϊ JavaScript ���飬����ÿ��Ԫ��Ҫô��ָ���Ĵ������ַ�����Ҫô��ָ������ֵ�� Number �� NativePointer��

- putBranchAddress(address): ���÷�֧/��ת��������ַ����Ĵ���

- canBranchDirectlyBetween(from, to): ȷ���������������ڴ�λ��֮���Ƿ���ܴ���ֱ�ӷ�֧

- putBImm(address): ��һ��Bָ��

- putBLabel(labelId): ����һ�� B ָ������labelId���ɹ�ȥ��δ������putLabel()

- putBCondLabel(cc, labelId): ����һ�� B COND ָ������labelId���ɹ�ȥ��δ������putLabel()

- putBlImm(address): ��һ��BLָ��

- putBlLabel(labelId): ����һ��BLָ������labelId���ɹ�ȥ��δ������putLabel()

- putBrReg(reg): ��һ�� BR ָ��

- putBrRegNoAuth(reg): ����һ�� BR ָ�����һ��û���κ���֤λ��ԭʼָ��

- putBlrReg(reg): ��һ�� BLR ָ��

- putBlrRegNoAuth(reg): ����һ�� BLR ָ�����һ��û���κ���֤λ��ԭʼָ��

- putRet(): ��һ��RETָ��

- putCbzRegLabel(reg, labelId): ����һ�� CBZ ָ������labelId���ɹ�ȥ��δ������putLabel()

- putCbnzRegLabel(reg, labelId): ����һ�� CBNZ ָ������labelId���ɹ�ȥ��δ������putLabel()

- putTbzRegImmLabel(reg, bit, labelId): ����һ�� TBZ ָ������labelId���ɹ�ȥ��δ������putLabel()

- putTbnzRegImmLabel(reg, bit, labelId): ����һ�� TBNZ ָ������labelId���ɹ�ȥ��δ������putLabel()

- putPushRegReg(regA, regB): ����һ��PUSHָ��

- putPopRegReg(regA, regB): ��һ��POPָ��

- putPushAllXRegisters(): ������ X �Ĵ���ѹ���ջ����Ĵ���

- putPopAllXRegisters(): ������ X �Ĵ����Ӷ�ջ�е�������Ĵ���

- putPushAllQRegisters(): �ѽ����� Q �Ĵ���ѹ���ջ����Ĵ���

- putPopAllQRegisters(): ������ Q �Ĵ����Ӷ�ջ�е�������Ĵ���

- putLdrRegAddress(reg, address): ��һ�� LDR ָ��

- putLdrRegU64(reg, val): ��һ�� LDR ָ��

- putLdrRegRef(reg): ����һ�����������������õ� LDR ָ�����һ����͸��������ֵ����ֵӦ�ô��ݵ�putLdrRegValue() �����λ��

- putLdrRegValue(ref, value): ����ֵ������ǰһ�� LDR ָ�� putLdrRegRef()

- putLdrRegRegOffset(dstReg, srcReg, srcOffset): ��һ�� LDR ָ��

- putLdrswRegRegOffset(dstReg, srcReg, srcOffset): ��һ�� LDRSW ָ��

- putAdrpRegAddress(reg, address): ��һ�� ADRP ָ��

- putStrRegRegOffset(srcReg, dstReg, dstOffset): ��һ��STRָ��

- putLdpRegRegRegOffset(regA, regB, regSrc, srcOffset, mode): ����һ�� LDP ָ��

- putStpRegRegRegOffset(regA, regB, regDst, dstOffset, mode): ��һ��STPָ��

- putMovRegReg(dstReg, srcReg): ��һ�� MOV ָ��

- putUxtwRegReg(dstReg, srcReg): ����һ�� UXTW ָ��

- putAddRegRegImm(dstReg, leftReg, rightValue): ��һ�� ADD ָ��

- putAddRegRegReg(dstReg, leftReg, rightReg): ��һ�� ADD ָ��

- putSubRegRegImm(dstReg, leftReg, rightValue): ����һ�� SUB ָ��

- putSubRegRegReg(dstReg, leftReg, rightReg): ����һ�� SUB ָ��

- putAndRegRegImm(dstReg, leftReg, rightValue): ����һ�� AND ָ��

- putTstRegImm(reg, immValue): ��һ�� TST ָ��

- putCmpRegReg(regA, regB): ��һ��CMPָ��

- putXpaciReg(reg): ��һ��XPACIָ��

- putNop(): ��һ�� NOP ָ��

- putBrkImm(imm): ��һ�� BRK ָ��

- putInstruction(insn): ��ԭʼָ����Ϊ JavaScript ���

- putBytes(data): ���ṩ��ArrayBuffer �з���ԭʼ����

- sign(value): ��������ָ��ֵǩ��

#### Arm64�ض�λ��
- `new Arm64Relocator(inputCode, output)`������һ���µĴ����ض�λ�������ڽ� AArch64 ָ���һ���ڴ�λ�ø��Ƶ���һ���ڴ�λ�ã�ע����Ӧ�ص�����λ����ص�ָ�Դ��ַ��inputCodeһ��NativePointerָ����Ŀ����ָ������Ŀ���ڴ��ַoutput��Arm64Writer������

- `reset(inputCode, output)`: ����ʵ��

- `dispose()`: ���е������ڴ�

- input:��ĿǰΪֹ�Ķ�������ָ�null ��ÿ�ε��� ʱ��ʼ������readOne()��

- eob: boolean ָʾ�Ƿ��ѵ����β���������ѵ����κ����͵ķ�֧���� CALL��JMP��BL��RET��

- eoi������ֵ��ָʾ�Ƿ��ѵ���������������磬�����ѵ��� JMP/B/RET��һ��ָ��֮�������Ҳ����û����Ч���롣

- readOne(): ����һ��ָ������ض�λ�����ڲ��������������ص�ĿǰΪֹ��ȡ���ֽ���������֮ǰ�ĵ��á������Լ������ô˷����Ա��ֻ��壬������������writeOne()��skipOne()�����ߣ������Ի��嵽����ĵ㣬Ȼ�����writeAll(). �����������ʱ�����㣬����ζ�Ÿ�eoi������ now true��

- peekNextWriteInsn(): ͵����һ��Ҫд���������ָ��

- peekNextWriteSource(): �鿴��һ��Ҫд���������ָ��ĵ�ַ

- skipOne(): ����������Ҫд��ָ��

- writeOne(): д��һ������ָ��

- writeAll(): д�����л���ָ��

#### AArch64 ö������
- �Ǽǣ� x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 x10 x11 x12 x13 x14 x15 x16 x17 x18 x19 x20 x21 x22 x23 x24 x25 x26 x27 x28 x29 x30 w0 w1 w2 w3 w4 w5 w6 w7 w8 w9 w10 w11 w12 w13 w14 w15 w16 w17 w18 w19 w20 w21 w22 w23 w24 w25 w26 w27 w28 w29 w30 sp lr fp wsp wzr xzr nzcv ip0 ip1 s0 s1 s2 s3 s4 s5 s6 s7 s8 s9 s10 s11 s12 s13 s14 s15 s16 s17 s18 s19 s20 s21 s22 s23 s24 s25 s26 s27 s28 s29 s30 s31 d0 d1 d2 d3 d4 d5 d6 d7 d8 d9 d10 d11 d12 d13 d14 d15 d16 d17 d18 d19 d20 d21 d22 d23 d24 d25 d26 d27 d28 d29 d30 d31 q0 q1 q2 q3 q4 q5 q6 q7 q8 q9 q10 q11 q12 q13 q14 q15 q16 q17 q18 q19 q20 q21 q22 q23 q24 q25 q26 q27 q28 q29 q30 q31

- �������룺 eq ne hs lo mi pl vs vc hi ls ge lt gt le al nv

- ����ģʽ�� post-adjust signed-offset pre-adjust

#### MipsWriter
- new MipsWriter(codeAddress[, { pc: ptr('0x1234') }])������һ���µĴ����д������������ֱ��д���ڴ�� MIPS �������� codeAddress��ָ��ΪNativePointer���ڶ���������һ����ѡ��ѡ��������п���ָ����ʼ��������������ڽ��������ɵ��ݴ滺����ʱ�����á���Memory.patchCode() �� iOS��ʹ��ʱ�Ǳز����ٵģ������ܻ�Ϊ���ṩһ����ʱλ�ã���λ���Ժ��ӳ�䵽Ԥ���ڴ�λ�õ��ڴ��С�

- reset(codeAddress[, { pc: ptr('0x1234') }]): ����ʵ��

- dispose(): ���е������ڴ�

- flush()��������ǩ���ò�������������д���ڴ档һ��������ɴ��룬��Ӧ��ʼ�յ�������ͨ��Ҳϣ���ڲ���صĴ����֮��ִ�д˲�����������һ�������ɶ������ʱ��

- base: �����һ���ֽڵ��ڴ�λ�ã���ΪNativePointer

- code: �������һ���ֽڵ��ڴ�λ�ã���ΪNativePointer

- pc: ���������һ���ֽڴ��ĳ������������ΪNativePointer

- offset: ��ǰƫ������Ϊ JavaScript ����

- skip(nBytes)�� ���� nBytes

- putLabel(id): �ڵ�ǰλ�÷�һ����ǩ������id��һ���ַ������ڹ�ȥ��- δ����put*Label()�����п��ܻᱻ����

- putCallAddressWithArguments(func, args): put ���ô���ָ���� C ��������Ĵ���args��ָ��Ϊ JavaScript ���飬����ÿ��Ԫ��Ҫô��ָ���Ĵ������ַ�����Ҫô��ָ������ֵ�� Number �� NativePointer��

- putCallRegWithArguments(reg, args): put ���ô���ָ���� C ��������Ĵ���args��ָ��Ϊ JavaScript ���飬����ÿ��Ԫ��Ҫô��ָ���Ĵ������ַ�����Ҫô��ָ������ֵ�� Number �� NativePointer��

- putJAddress(address): ��һ��Jָ��

- putJAddressWithoutNop(address): ��һ�� J WITHOUT NOP ָ��

- putJLabel(labelId): ����һ�� J ָ������labelId���ɹ�ȥ��δ������putLabel()

- putJrReg(reg): ��һ��JRָ��

- putJalAddress(address): ��һ�� JAL ָ��

- putJalrReg(reg): ��һ�� JALR ָ��

- putBOffset(offset): ��һ��Bָ��

- putBeqRegRegLabel(rightReg, leftReg, labelId): ����һ�� BEQ ָ������labelId���ɹ�ȥ��δ������putLabel()

- putRet(): ��һ��RETָ��

- putLaRegAddress(reg, address): ��һ�� LA ָ��

- putLuiRegImm(reg, imm): ��һ��LUIָ��

- putDsllRegReg(dstReg, srcReg, amount): ��һ��DSLLָ��

- putOriRegRegImm(rt, rs, imm): ����һ�� ORI ָ��

- putLdRegRegOffset(dstReg, srcReg, srcOffset): ��һ��LDָ��

- putLwRegRegOffset(dstReg, srcReg, srcOffset): ��һ��LWָ��

- putSwRegRegOffset(srcReg, dstReg, dstOffset): ��һ��SWָ��

- putMoveRegReg(dstReg, srcReg): ����һ�� MOVE ָ��

- putAdduRegRegReg(dstReg, leftReg, rightReg): ��һ�� ADDU ָ��

- putAddiRegRegImm(dstReg, leftReg, imm): ��һ�� ADDI ָ��

- putAddiRegImm(dstReg, imm): ��һ�� ADDI ָ��

- putSubRegRegImm(dstReg, leftReg, imm): ����һ�� SUB ָ��

- putPushReg(reg): ����һ��PUSHָ��

- putPopReg(reg): ��һ��POPָ��

- putMfhiReg(reg): ��һ��MFHIָ��

- putMfloReg(reg): ��һ��MFLOָ��

- putMthiReg(reg): ��һ�� MTHI ָ��

- putMtloReg(reg): ��һ�� MTLO ָ��

- putNop(): ��һ�� NOP ָ��

- putBreak(): ��һ�� BREAK ָ��

- putPrologueTrampoline(reg, address): ����һ����С�ߴ�ıĴ�������������������ַ

- putInstruction(insn): ��ԭʼָ����Ϊ JavaScript ���

- putBytes(data): ���ṩ��ArrayBuffer �з���ԭʼ����

#### Mips�ض�λ��
- new MipsRelocator(inputCode, output)������һ���µĴ����ض�λ�������ڽ� MIPS ָ���һ���ڴ�λ�ø��Ƶ���һ���ڴ�λ�ã�ע����Ӧ�ص�����λ����ص�ָ�Դ��ַ��inputCodeһ��NativePointerָ����Ŀ����output��ָ������Ŀ���ڴ��ַ��MipsWriter������

- reset(inputCode, output): ����ʵ��

- dispose(): ���е������ڴ�

- input:��ĿǰΪֹ�Ķ�������ָ�null ��ÿ�ε��� ʱ��ʼ������readOne()��

- eob: boolean ָʾ�Ƿ��ѵ����β���������ѵ����κ����͵ķ�֧���� CALL��JMP��BL��RET��

- eoi������ֵ��ָʾ�Ƿ��ѵ���������������磬�����ѵ��� JMP/B/RET��һ��ָ��֮�������Ҳ����û����Ч���롣

- readOne(): ����һ��ָ������ض�λ�����ڲ��������������ص�ĿǰΪֹ��ȡ���ֽ���������֮ǰ�ĵ��á������Լ������ô˷����Ա��ֻ��壬������������writeOne()��skipOne()�����ߣ������Ի��嵽����ĵ㣬Ȼ�����writeAll(). �����������ʱ�����㣬����ζ�Ÿ�eoi������ now true��

- peekNextWriteInsn(): ͵����һ��Ҫд���������ָ��

- peekNextWriteSource(): �鿴��һ��Ҫд���������ָ��ĵ�ַ

- skipOne(): ����������Ҫд��ָ��

- writeOne(): д��һ������ָ��

- writeAll(): д�����л���ָ��

#### MIPS ö������
- �Ǽǣ� v0 v1 a0 a1 a2 a3 t0 t1 t2 t3 t4 t5 t6 t7 s0 s1 s2 s3 s4 s5 s6 s7 t8 t9 k0 k1 gp sp fp s8 ra hi lo zero at 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31

#### console
- `console.log(line), console.warn(line), console.error(line)` ���׼�����������д��line�ַ��������磺ʹ��Frida-Python��ʱ�������� stdout ���� stderr��ʹ��frida-qml��ʱ���������qDebug������������һ��ArrayBuffer������Ĭ�ϲ����Զ�����hexdump���и�ʽ�������

ArrayBuffer����Ĳ�������hexdump()Ĭ��ѡ��Ľ���滻��

#### ʮ������ת��
- `hexdump(target[, options])` ��һ��ArrayBuffer����NativePointer��target����, ���ո��Ӳ���optionָ���ĸ�ʽ���

���磺
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
- `int64(v)` new Int64(v)����д��ʽ
- `uint64(v)` new UInt64(v)����д��ʽ
- `ptr(s)` new NativePointer(s)����д��ʽ
- `NULL` ptr("0")����д��ʽ

#### ������ע�����֮���ͨ��
- `recv([type, ]callback)`��ע��ص�callback, �����ܵ���Ϣʱ, ��ָ��ص���������ѡ����type���ڶ���Ϣ�Ĺ��ˣ�ֻ���ܶ�Ӧtype����Ϣ�����յ�һ����Ϣ�������Ҫ����������Ϣ����Ҫ�ٴε���recv
- `send(message[, data])` ��Ŀ����̷���message(��������л�ΪJson), ������и���������Ҫ���ͣ�����ʹ��data(�������ͱ�����ArrayBuffer����Ϊ��������(��ֵΪ0-255)) ��Ȼsend()���첽�ģ������͵�����Ϣ���ܿ�����δ��Ը�Ƶ�����Ż�����˿��Խ������Ϣ�ݴ��������ͣ��Ӷ���ȡ���͵���ʱ�͸��ߵ���������
- `rpc.exports` �����ڳ����е���RPC����APi����, Keyָ�����������ƣ�ValueΪָ�������ĺ�������������ֱ�ӷ���һ��ֵ��Ҳ�������첽��ʽ��Promise�ķ�ʽ���ء�
���磺
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

����Python���ض˿���ʹ�����������Ľű�ʹ�õ�����RPC������
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
�������ʾ���У�����ʹ��`script.on('message', on_message)`��������Ŀ����̵��κ���Ϣ����Ϣ��ؿ�������`script`��`session`�������档���磬���������Ŀ������˳�ʱ������ʹ�� `session.on('detached', your_function)`.

#### ��ʱ�¼�
- `setTimeout(func, delay[, ...parameters])` �ӳ�delay��������func, ����ѡ�񴫵�һ������parameters������ֵΪһ��ID����ID���Դ��ݸ�clearTimeoutȡ����func�����ĵ���
- `clearTimeout(id)` ȡ��ͨ��setTimeout����ĵ���
- `setInterval(func, delay[, ...parameters])`:ÿdelay�������һ��func������ѡ�񴫵�һ������parameters������ֵΪһ��ID, ��ID���Դ���clearIntervalȡ����func�����ĵ���
- `clearInterval(id)` ȡ��ͨ��setTimeout����ĵ���
- `setImmediate(func[, ...parameters])`: ������Frida��JavaScript�߳��ϵ��ã���ѡ�񴫵�һ������parameters������ֵΪһ��ID, ��ID���Դ���clearImmediateȡ����func�����ĵ���
- `clearImmediate(id)` ȡ��ͨ��setImmediate����ĵ���

#### ��������
- `gc()`: ǿ���������ա��Բ��Ժ����ã��������漰`WeakRef.bind()`.












































