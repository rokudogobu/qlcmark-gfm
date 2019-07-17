/*
 
 Copyright (c) 2019 rokudogobu
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 
 */

document.addEventListener('DOMContentLoaded', function(){
	var uls = document.getElementsByTagName('ul');
	for ( var i = 0; i < uls.length; i++ ) {
		var ul = uls.item(i);
		if ( ul.childElementCount == 0 ) continue;
		
		var is_tasklist = true;
		var lis = ul.children;
		for ( var j = 0; j < lis.length; j++ ) {
			var li = lis.item(j);
			var is_taskitem = false;
			do {
				if ( li.tagName != 'LI'        ) break;
				if ( li.childElementCount == 0 ) break;
				
				var inputs = li.children;
				for ( var k = 0; k < inputs.length; k++ ) {
					var input = inputs.item( k );
                    
					if ( input.tagName != 'INPUT'                   ) continue;
					if ( ! input.hasAttribute( 'disabled' )         ) continue;
					if ( input.getAttribute( 'type' ) != 'checkbox' ) continue;
                    
					is_taskitem = true;
					break;
				}
			} while ( false );

			if ( ! is_taskitem ) {
				is_tasklist = false;
				break;
			}
		}
		
		if ( is_tasklist ) {
			ul.classList.add('tasklist');
		}
	}
});
