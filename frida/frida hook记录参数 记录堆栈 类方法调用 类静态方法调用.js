function main(){
    Java.perform(function(){
        /*Java.use("com.example.lesson4one.MainActivity") 获取类实例*/
        /*重载函数.overload(参数类型)*/
        Java.use("com.example.lesson4one.MainActivity").fun.overload('int', 'int').implementation = function(arg1,arg2)
        {
            /*执行原始函数*/
            var result = this.fun(arg1,arg2);
            /*打印调用堆栈*/
            console.log(Java.use("android.util.Log").getStackTraceString(Java.use("java.lang.Throwable").$new()));
            /*打印参数和返回值*/
            console.log("arg1,arg2,result",arg1,arg2,result)
            /*修改返回值*/
            return 800;
        }

        /*构造函数 获取类实例后 使用$new()方法调用构造函数*/
        Java.use("com.example.lesson4one.MainActivity").fun.overload('java.lang.String').implementation = function (arg1 ){
            var result = this.fun(Java.use('java.lang.String').$new("NIHAOJAVA"));
            console.log("arg1,result",arg1,result)
            return Java.use('java.lang.String').$new("NIHAOJS");
        }
        
        /*调用类实例方法*/
        Java.choose("com.example.lesson4one.MainActivity",{
            onMatch:function(instance){
                console.log("found instance :",instance)
                console.log("found instance :",instance.secret())
            },onComplete:function(){}
        })
       
        /*调用类静态方法*/
        var result = Java.use("com.example.lesson4one.MainActivity").secret2();
        console.log(result);
    
        Java.use("net.sqlcipher.database.SQLiteOpenHelper").getWritableDatabase.overload('java.lang.String').implementation = function (x){
           var result = this.getWritableDatabase(x);
           console.log(Java.use("android.util.Log").getStackTraceString(Java.use("java.lang.Throwable").$new()));
           console.log("xString,result",x,result);
           return result;
        }

       Java.use("net.sqlcipher.database.SQLiteOpenHelper").getWritableDatabase.overload('[C').implementation = function (x){
        var result = this.getWritableDatabase(x);
        console.log("xCharSe,result",x,result);
        return result;
    }
    })
}
setImmediate(main)

function invoke(){
    Java.perform(function(){
        Java.choose("com.example.yaphetshan.tencentwelcome.MainActivity",{
            onMatch:function(instance){
                console.log("found insttance ",instance);
                console.log("invoke instance.a ",instance.a());
            },onComplete:function(){console.log("search completed !")}
        })
    })
}

setTimeout(invoke,2000)