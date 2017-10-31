// 設定とか。
Print("設定ファイルが読み込まれました。\n");


Aen.Print("AEN を使っています。\n");


var array = Aen.Call("EvLoadGraph", "./data/graphic/bg/0001.png");


for (i = 0; i < array.length; i++) {
	Aen.Print("Print -> " + i + ":" + array[i] + "\n");
	Aen.Call("EvDrawGraph", 10, 50, array[i]);
}


