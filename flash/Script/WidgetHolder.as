import com.greensock.TimelineLite;
import com.greensock.easing.*;

class Script.WidgetHolder extends MovieClip
{
	var AnimHolder:Object;
	var MenuAnim:TimelineLite;
	
	public function WidgetHolder() 
	{
		// constructor code
		_visible = true;
		AnimHolder = new Object();
		MenuAnim = new TimelineLite;
	}
	
	public function LoadWidget(_id:Number, _path:String, _xPos:Number, _yPos:Number, _xWidth:Number, _yHeight:Number, _nAlpha:Number) : Void
	{
		var mcHolder:MovieClip;
		var id = to_string(_id);
		mcHolder = this.createEmptyMovieClip(id, _id);
		this[id].createEmptyMovieClip(id, _id);
		this[id]._x = _xPos;
		this[id]._y = _yPos;
		this[id]._alpha = 0;
		
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
		
		//this[id]._alpha = 0;
		if (AnimHolder[id]) {
			AnimHolder[id].restart();
		} else {
			AnimHolder[id] = new TimelineLite;
			AnimHolder[id].to(this[id], 0.25, {_alpha:_nAlpha});
		}
	}
	
	public function LoadText(_id:Number, _text:String, _font:String, _xPos:Number, _yPos:Number, _align:Number, _size:Number, _nAlpha:Number, _shadow:Boolean) : Void
	{
		var mcHolder:MovieClip;
		var id = to_string(_id);

		mcHolder = this.createEmptyMovieClip(id, _id);
		this[id].createTextField(id, _id, 0, 0, 100, 100);
		this[id]._x = _xPos;
		this[id]._y = _yPos;
		this[id]._alpha = 100;
		this[id][id].embedFonts = true;
		this[id][id].setNewTextFormat(new TextFormat(_font, _size, 0xFFFFFF));
		if (_align == 0) {
			this[id][id].autoSize = "left";
			this[id][id].align = "left";
		} else if (_align == 1) {
			this[id][id].autoSize = "right";
			this[id][id].align = "right";
		} else {
			this[id][id].autoSize = "center";
			this[id][id].align = "center";
		}
		this[id][id].text = _text;
		this[id][id].alpha = 100;
		if (_shadow) {
			this[id][id].filters = [new flash.filters.DropShadowFilter(6.0,45,0x000000,1.0,4.0,4.0,1.0,3,false,false,false)];
		}
	}
	
	public function UnloadWidget(_id:Number) : Void
	{
		var id = to_string(_id);
		this[id].swapDepths(0);
    	this[id].removeMovieClip();
		AnimHolder[id].kill();
		delete AnimHolder[id];
	}
	
	public function UnloadText(_id:Number) : Void
	{
		var id = to_string(_id);
		this[id].swapDepths(0);
    	this[id].removeMovieClip();
	}
	
	public function SetText(_id:Number, _text:String) : Void
	{
		var id = to_string(_id);
		this[id][id].text = _text;
		// UpdateCenter(id);
	}
	
	public function SetMenuAlpha(_nAlpha:Number) : Void
	{
		MenuAnim.kill();
		delete MenuAnim;
		MenuAnim = new TimelineLite;
		this._alpha = _nAlpha;
	}
	
	public function MenuFadeIn() : Void
	{
		MenuAnim.kill();
		delete MenuAnim;
		if (MenuAnim) {
			MenuAnim.restart();
		} else {
			MenuAnim = new TimelineLite;
			MenuAnim.to(this, 0.5, {_alpha:100});
		}
	}
	
	public function MenuFadeOut() : Void
	{
		MenuAnim.kill();
		delete MenuAnim;
		if (MenuAnim) {
			MenuAnim.restart();
		} else {
			MenuAnim = new TimelineLite;
			MenuAnim.to(this, 0.5, {_alpha:0});
		}
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
