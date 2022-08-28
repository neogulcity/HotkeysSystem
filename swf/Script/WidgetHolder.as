import com.greensock.TimelineLite;
import com.greensock.easing.*;

class Script.WidgetHolder extends MovieClip
{
	var IDHolder:Number;
	
	public function WidgetHolder() 
	{
		// constructor code
		_visible = true;
		IDHolder = 0;
	}
	
	public function LoadWidget(_path:String, _xPos:Number, _yPos:Number, _xWidth:Number, _yHeight:Number, _nAlpha:Number) : String
	{
		var mcHolder:MovieClip;
		var id: String;
		id = to_string(IDHolder);
		IDHolder += 1;
		mcHolder = this.createEmptyMovieClip(id, this.getNextHighestDepth());
		this[id].createEmptyMovieClip(id, this.getNextHighestDepth());
		this[id]._x = _xPos;
		this[id]._y = _yPos;
		this[id]._alpha = 100;
		
		var loader:MovieClipLoader = new MovieClipLoader();
		var listener:Object = new Object();
		loader.addListener(listener);
		loader.loadClip(_path, this[id][id]);
		listener.onLoadInit = function(mc:MovieClip, param:Number) {
			mcHolder._width = _xWidth;
			mcHolder._height = _yHeight;
			mcHolder._alpha = _nAlpha;
			
			mc._x = 0-(mc._width/2);
			mc._y = 0-(mc._height/2);
		}

		return(id);
	}
	
	public function LoadText(_text:String, _font:String, _size:Number, _xPos:Number, _yPos:Number) : String
	{
		var id: String;
		id = to_string(IDHolder);
		IDHolder += 1;

		this.createEmptyMovieClip(id, this.getNextHighestDepth());
		this[id].createTextField(id, this.getNextHighestDepth(), 0, 0, 100, 100);
		this[id]._x = _xPos;
		this[id]._y = _yPos;
		this[id]._alpha = 0;
		this[id][id].embedFonts = true;
		this[id][id].setNewTextFormat(new TextFormat(_font, _size, 0xFFFFFF));
		this[id][id].autoSize = "center";
		this[id][id].align = "center";
		this[id][id].text = _text;
		UpdateCenter(id);
		
		return(id);
	}
	
	public function UnloadWidget(_id:Number) : Void
	{
		var id = to_string(_id);
		this[id].swapDepths(0);
    	this[id].removeMovieClip();
	}
	
	public function UnloadText(_id:Number) : Void
	{
		var id = to_string(_id);
		this[id].swapDepths(0);
    	this[id].removeTextField();
	}
	
	public function SetText(_id:Number, _text:String) : Void
	{
		var id = to_string(_id);
		this[id][id].text = _text;
		UpdateCenter(id);
	}
	
	public function SetSize(_id:Number, _xWidth:Number, _yHeight:Number) : Void
	{
		var id = to_string(_id);
		this[id]._width = _xWidth;
		this[id]._height = _yHeight;
		UpdateCenter(id);
	}

	public function SetPos(_id:Number, _xPos:Number, _yPos:Number) : Void
	{
		var id = to_string(_id);
		UpdateCenter(id);
		this[id]._x = _xPos;
		this[id]._y = _yPos;
	}
	
	public function SetAlpha(_id:Number, _nAlpha:Number) : Void
	{
		var id = to_string(_id);
		this[id]._alpha = _nAlpha;
	}
	
	public function SetMenuAlpha(_nAlpha:Number) : Void
	{
		this._alpha = _nAlpha;
	}
	
	public function Animate(_id:Number) : Void
	{
		var id = to_string(_id);
				
		var TweenLite: TimelineLite;
		TweenLite = new TimelineLite;
		TweenLite.to(this[id][id], 0.13, {_alpha:0});
		TweenLite.to(this[id][id], 0.12, {_alpha:100, onComplete:play, onCompleteScope:this[id]});
		delete TweenLite;
	}
	
	public function DissolveIn(_) : Void
	{		
		var TweenLite: TimelineLite;
		TweenLite = new TimelineLite;
		TweenLite.to(this, 1.0, {_alpha:0});
		delete TweenLite;
	}
	
	public function DissolveOut(_) : Void
	{		
		var TweenLite: TimelineLite;
		TweenLite = new TimelineLite;
		TweenLite.to(this, 1.0, {_alpha:100});
		delete TweenLite;
	}
	
	public function Clear() : Void
	{
		IDHolder = 0;
	}
	
	function to_string(_id:Number) : String
	{
		var blank = "";
		return (blank+_id);
	}
	
	function UpdateCenter(id: String):Void {
		this[id][id]._x = 0-(this[id][id]._width/2);
		this[id][id]._y = 0-(this[id][id]._height/2);
	}
}
