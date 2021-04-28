/*类加载器解释参考：https://www.jianshu.com/p/efd19125255c*/

function main(){
    /**重载方法HOOK 记录参数 修改返回值*/
    Java.perform(function(){
        Java.use("com.example.androiddemo.Activity.LoginActivity").a.overload('java.lang.String', 'java.lang.String').implementation = function(x,y){
            var result = this.a(x,y);
            console.log("x,y,result",x,y,result);
            return result;
        }

        Java.use("com.example.androiddemo.Activity.FridaActivity1").a.implementation = function (x){
            console.log("first challenge 1")
            return "R4jSLLLLLLLLLLOrLE7/5B+Z6fsl65yj6BgC6YWz66gO6g2t65Pk6a+P65NK44NNROl0wNOLLLL=" ;
        }
    })
}

function second(){
    Java.perform(function(){
        /**静态方法*/
        Java.use("com.example.androiddemo.Activity.FridaActivity2").setStatic_bool_var();
        /**普通方法*/
        Java.choose("com.example.androiddemo.Activity.FridaActivity2",{
            onMatch:function(instance){
                console.log("found instance :",instance);
                instance.setBool_var()
            },onComplete:function(){}
        })
    })
}

function third(){
    Java.perform(function(){
        /**类静态变量*/
        Java.use("com.example.androiddemo.Activity.FridaActivity3").static_bool_var.value = true ;
        /**类成员变量*/
        Java.choose("com.example.androiddemo.Activity.FridaActivity3",{
            onMatch:function(instance){
                console.log("found instance :",instance);
                instance.bool_var.value = true ;
                instance._same_name_bool_var.value = true ;
            },onComplete:function(){}
        })

    })
}

function forth(){
    Java.perform(function(){
        /*HOOK 内部类方法*/
        Java.use("com.example.androiddemo.Activity.FridaActivity4$InnerClasses").check1.implementation = function(){return true};
        Java.use("com.example.androiddemo.Activity.FridaActivity4$InnerClasses").check2.implementation = function(){return true};
        Java.use("com.example.androiddemo.Activity.FridaActivity4$InnerClasses").check3.implementation = function(){return true};
        Java.use("com.example.androiddemo.Activity.FridaActivity4$InnerClasses").check4.implementation = function(){return true};
        Java.use("com.example.androiddemo.Activity.FridaActivity4$InnerClasses").check5.implementation = function(){return true};
        Java.use("com.example.androiddemo.Activity.FridaActivity4$InnerClasses").check6.implementation = function(){return true};
    })
}

function forth2(){
    Java.perform(function(){
        var class_name = "com.example.androiddemo.Activity.FridaActivity4$InnerClasses" ;
        var InnerClass = Java.use(class_name);
        /*java 反射获取获取内部类的方法*/
        var all_methods = InnerClass.class.getDeclaredMethods();
        for(var i = 0; i < all_methods.length; i++){
            var method = all_methods[i];
            /*通过字符串分割找到方法名称*/
            var substring = method.toString().substr(method.toString().indexOf(class_name) + class_name.length + 1);
            var finalMethodString = substring.substr(0,substring.indexOf("("));
            console.log(finalMethodString);
            InnerClass[finalMethodString].implementation = function(){return true};
        }
    })
}

function fifth(){
    Java.perform(function(){
        Java.choose("com.example.androiddemo.Activity.FridaActivity5",{
            onMatch:function(instance){
                /*根据类实例获取类名称*/
                console.log("found instance getDynamicDexCheck :",instance.getDynamicDexCheck().$className);
            },onComplete:function(){console.log("search complete!")}
        })
        /**Java类加载器*/
        Java.enumerateClassLoaders({
            onMatch:function(loader){
                try {
                    /*找到对应类的类加载器*/
                    if(loader.findClass("com.example.androiddemo.Dynamic.DynamicCheck")){
                        console.log("Succefully found loader!",loader);
                        Java.classFactory.loader = loader;
                    }
                } catch (error) {
                    console.log("found error "+error)
                    
                }
            },onComplete:function(){"enum completed!"}
        })
        Java.use("com.example.androiddemo.Dynamic.DynamicCheck").check.implementation = function(){return true};
    })
}

function sixth (){
    Java.perform(function(){
        Java.use("com.example.androiddemo.Activity.Frida6.Frida6Class0").check.implementation = function (){return true };
        Java.use("com.example.androiddemo.Activity.Frida6.Frida6Class1").check.implementation = function (){return true };
        Java.use("com.example.androiddemo.Activity.Frida6.Frida6Class2").check.implementation = function (){return true };

        
    })
    
}

function sixth2(){
    Java.perform(function(){
        Java.enumerateLoadedClasses({
            onMatch:function(name,handle){
                if(name.toString().indexOf("com.example.androiddemo.Activity.Frida6.Frida6")>=0){
                    console.log("name",name)
                    Java.use(name).check.implementation = function(){return true}
                }
            },onComplete(){}
        })

    })

}

setImmediate(sixth2);