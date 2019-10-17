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
	Array.from( document.getElementsByTagName( 'ul' ) ).forEach( ( ul ) => {
		let is_tasklist = Array.from( ul.children ).reduce( ( flag, li ) => {
			if ( ! flag                                                      ) return false;
			if (   li.tagName != 'LI'                                        ) return false;
			if ( ! li.childNodes.length >= 2                                 ) return false;
			if (   li.childNodes[0].nodeType != Node.ELEMENT_NODE            ) return false;
			if (   li.childNodes[0].nodeName != 'INPUT'                      ) return false;
			if ( ! li.firstElementChild.hasAttribute( 'disabled' )           ) return false;
			if (   li.firstElementChild.getAttribute( 'type' ) != 'checkbox' ) return false;
			if (   li.childNodes[1].nodeType != Node.TEXT_NODE               ) return false;
			return true;
		}, true );
		
		if ( is_tasklist ) {
			ul.classList.add( 'tasklist' ); 
			Array.from( ul.children ).forEach( ( li ) => {
				let label = document.createElement( 'label' );
				label.innerHTML = li.childNodes[1].nodeValue;
				li.replaceChild( label, li.childNodes[1] );
				li.classList.add( 'tasklistitem' );
				li.children[0].classList.add( 'tasklistitem-checkbox' );
				li.children[1].classList.add( 'tasklistitem-label' );
			} );
		}
	} );
});
