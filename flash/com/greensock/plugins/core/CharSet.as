/**
 * VERSION: 1.0.0
 * DATE: 2013-03-27
 * AS2
 * UPDATES AND DOCS AT: http://www.greensock.com
 **/
/**
 * <p><strong>See AS3 files for full ASDocs</strong></p>
 * 
 * <p><strong>Copyright 2008-2014, GreenSock. All rights reserved.</strong> This work is subject to the terms in <a href="http://www.greensock.com/terms_of_use.html">http://www.greensock.com/terms_of_use.html</a> or for <a href="http://www.greensock.com/club/">Club GreenSock</a> members, the software agreement that was issued with the membership.</p>
 * 
 * @author Jack Doyle, jack@greensock.com
 */
class com.greensock.plugins.core.CharSet {
		public var chars:Array;
		public var sets:Array;
		public var length:Number;
		
		public function CharSet(chars:String) {
			this.chars = chars.split("");
			this.sets = [];
			this.length = 50;
			var i:Number;
			for (i = 0; i < 20; i++) {
				sets[i] = _scrambleText(80, this.chars); //we create 20 strings that are 80 characters long, randomly chosen and pack them into an array. We then randomly choose the scrambled text from this array in order to greatly improve efficiency compared to creating new randomized text from scratch each and every time it's needed. This is a simple lookup whereas the other technique requires looping through as many times as there are characters needed, and calling Math.random() each time through the loop, building the string, etc.
			}
		}
		
		private static function _scrambleText(length:Number, chars:Array):String {
			var l:Number = chars.length,
				s:String = "";
			while (--length > -1) {
				s += chars[ ((Math.random() * l) | 0) ];
			}
			return s;
		}
		
		public function grow(newLength:Number):Void { //if we encounter a tween that has more than 80 characters, we'll need to add to the character sets accordingly. Once it's cached, it'll only need to grow again if we exceed that new length. Again, this is an efficiency tactic.
			var i:Number;
			for (i = 0; i < 20; i++) {
				sets[i] += _scrambleText(newLength - length, chars);
			}
			length = newLength;
		}
}