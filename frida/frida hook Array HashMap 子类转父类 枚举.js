function main() {
    Java.perform(function () {
        /*Java Array*/
        Java.openClassFile("/data/local/tmp/r0gson.dex").load();
        const gson = Java.use('com.r0ysue.gson.Gson');

        Java.use("java.lang.Character").toString.overload('char').implementation = function(x){
            var result = this.toString(x);
            console.log("x, result", x, result);
            return "对象数组";
        }

        Java.use("java.util.Arrays").toString.overload('[C').implementation = function (x){
            var charArray = Java.array('char', [ '一', '二', '三', '四','五' ]);
            var result = this.toString(charArray);
            console.log("x, result", gson.$new().toJson(charArray), result);            
            return Java.use('java.lang.String').$new(Java.array('char', ['一', '二', '三', '四','五']));
        }

        Java.use("java.util.Arrays").toString.overload('[B').implementation = function (x){ 
            var result = this.toString(x);
            console.log("x, result",gson.$new().toJson(x), result);            
            console.log("x, result", x, result);            
            return result;
        }

        /*类型转换：子类转化为父类*/
        var JuiceHandle = null ;
        Java.choose("com.r0ysue.a0526printout.Juice",{
            onMatch:function(instance){
                console.log("found instance :",instance);
                console.log("filling energy,",instance.fillEnergy());
                JuiceHandle= instance;
            },onComplete:function(){"Search Completed!"}
        })
        var WaterHandle = Java.cast(JuiceHandle ,Java.use("com.r0ysue.a0526printout.Water"));
        console.log("Water invoke still ", WaterHandle.still(WaterHandle));
        
        /*根据java接口创建接口类*/
        var beer = Java.registerClass({
            //创建类名称
            name: 'com.r0ysue.a0526printout.beer',  
            //接口类名称 
            implements: [Java.use('com.r0ysue.a0526printout.liquid')],
            methods: {
                flow: function () {
                    console.log("look I`m beer!");
                    return "taste good!";
                }
            }
        });
        console.log("beer.flow:",beer.$new().flow());  

        /*Java中枚举也是一种受限制的类*/
        Java.choose("com.r0ysue.a0526printout.Signal",{
            onMatch:function(instance){
                console.log("found instance:",instance)
                console.log("invoke getDeclaringClass",instance.getDeclaringClass())
            },onComplete:function(){console.log("Search Completed!")}
        })
        
        /*Java HashMap*/
        Java.choose("java.util.HashMap",{
            onMatch:function(instance){
                console.log("found HashMap",instance);
                console.log("HashMap toString:",instance.toString());
            },onComplete:function(){console.log("Search Completed!")}
        })
        
        Java.use("java.util.HashMap").put.implementation = function(x,y){
            var result = this.put(x, y);
            console.log("x,y,result",x,y,result)
            return result;
        }
    })
}
