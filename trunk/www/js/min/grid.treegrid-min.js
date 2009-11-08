(function(a){a.fn.extend({setTreeNode:function(b,c){return this.each(function(){var k=this;if(!k.grid||!k.p.treeGrid){return}var p=0,l=0;if(!k.p.expColInd){for(var r in k.p.colModel){if(k.p.colModel[r].name==k.p.ExpandColumn){p=l;k.p.expColInd=p;break}l++}if(!k.p.expColInd){k.p.expColInd=p}}else{p=k.p.expColInd}var n=k.p.treeReader.expanded_field;var g=k.p.treeReader.leaf_field;var f=k.p.treeReader.level_field;c.level=b[f];if(k.p.treeGridModel=="nested"){c.lft=b[k.p.treeReader.left_field];c.rgt=b[k.p.treeReader.right_field];if(!b[g]){b[g]=(parseInt(c.rgt,10)===parseInt(c.lft,10)+1)?"true":"false"}}else{c.parent_id=b[k.p.treeReader.parent_id_field]}var m=(b[n]&&b[n]=="true")?true:false;var o=parseInt(c.level,10);var j,q;if(k.p.tree_root_level===0){j=o+1;q=o}else{j=o;q=o-1}var d=document.createElement("div");a(d).addClass("tree-wrap").width(j*18);var h=document.createElement("div");a(h).css("left",q*18);d.appendChild(h);if(b[g]=="true"){a(h).addClass("tree-leaf");c.isLeaf=true}else{if(b[n]=="true"){a(h).addClass("tree-minus treeclick");c.expanded=true}else{a(h).addClass("tree-plus treeclick");c.expanded=false}}if(parseInt(b[f],10)!==parseInt(k.p.tree_root_level,10)){if(!a(k).isVisibleNode(c)){a(c).css("display","none")}}var e=a("td:eq("+p+")",c).html();var s=a("td:eq("+p+")",c).html("<span>"+e+"</span>").prepend(d);a(".treeclick",s).click(function(u){var t=u.target||u.srcElement;var i=a(t,k.rows).parents("tr:first")[0].rowIndex;if(!k.rows[i].isLeaf){if(k.rows[i].expanded){a(k).collapseRow(k.rows[i]);a(k).collapseNode(k.rows[i])}else{a(k).expandRow(k.rows[i]);a(k).expandNode(k.rows[i])}}return false});if(k.p.ExpandColClick===true){a("span",s).css("cursor","pointer").click(function(u){var t=u.target||u.srcElement;var i=a(t,k.rows).parents("tr:first")[0].rowIndex;if(!k.rows[i].isLeaf){if(k.rows[i].expanded){a(k).collapseRow(k.rows[i]);a(k).collapseNode(k.rows[i])}else{a(k).expandRow(k.rows[i]);a(k).expandNode(k.rows[i])}}a(k).setSelection(k.rows[i].id);return false})}})},setTreeGrid:function(){return this.each(function(){var b=this;if(!b.p.treeGrid){return}a.extend(b.p,{treedatatype:null});if(b.p.treeGridModel=="nested"){b.p.treeReader=a.extend({level_field:"level",left_field:"lft",right_field:"rgt",leaf_field:"isLeaf",expanded_field:"expanded"},b.p.treeReader)}else{if(b.p.treeGridModel=="adjacency"){b.p.treeReader=a.extend({level_field:"level",parent_id_field:"parent",leaf_field:"isLeaf",expanded_field:"expanded"},b.p.treeReader)}}})},expandRow:function(b){this.each(function(){var d=this;if(!d.grid||!d.p.treeGrid){return}var c=a(d).getNodeChildren(b);a(c).each(function(e){a(this).css("display","");if(this.expanded){a(d).expandRow(this)}})})},collapseRow:function(b){this.each(function(){var d=this;if(!d.grid||!d.p.treeGrid){return}var c=a(d).getNodeChildren(b);a(c).each(function(e){a(this).css("display","none");a(d).collapseRow(this)})})},getRootNodes:function(){var b=[];this.each(function(){var d=this;if(!d.grid||!d.p.treeGrid){return}switch(d.p.treeGridModel){case"nested":var c=d.p.treeReader.level_field;a(d.rows).each(function(e){if(parseInt(this[c],10)===parseInt(d.p.tree_root_level,10)){b.push(this)}});break;case"adjacency":a(d.rows).each(function(e){if(this.parent_id.toLowerCase()=="null"){b.push(this)}});break}});return b},getNodeDepth:function(c){var b=null;this.each(function(){var d=this;if(!this.grid||!this.p.treeGrid){return}switch(d.p.treeGridModel){case"nested":b=parseInt(c.level,10)-parseInt(this.p.tree_root_level,10);break;case"adjacency":b=a(d).getNodeAncestors(c).length;break}});return b},getNodeParent:function(c){var b=null;this.each(function(){var g=this;if(!g.grid||!g.p.treeGrid){return}switch(g.p.treeGridModel){case"nested":var e=parseInt(c.lft,10),d=parseInt(c.rgt,10),f=parseInt(c.level,10);a(this.rows).each(function(){if(parseInt(this.level,10)===f-1&&parseInt(this.lft)<e&&parseInt(this.rgt)>d){b=this;return false}});break;case"adjacency":a(this.rows).each(function(){if(this.id===c.parent_id){b=this;return false}});break}});return b},getNodeChildren:function(c){var b=[];this.each(function(){var h=this;if(!h.grid||!h.p.treeGrid){return}switch(h.p.treeGridModel){case"nested":var f=parseInt(c.lft,10),e=parseInt(c.rgt,10),g=parseInt(c.level,10);var d=c.rowIndex;a(this.rows).slice(1).each(function(j){if(parseInt(this.level,10)===g+1&&parseInt(this.lft,10)>f&&parseInt(this.rgt,10)<e){b.push(this)}});break;case"adjacency":a(this.rows).slice(1).each(function(j){if(this.parent_id==c.id){b.push(this)}});break}});return b},getFullTreeNode:function(c){var b=[];this.each(function(){var h=this;if(!h.grid||!h.p.treeGrid){return}switch(h.p.treeGridModel){case"nested":var f=parseInt(c.lft,10),e=parseInt(c.rgt,10),g=parseInt(c.level,10);var d=c.rowIndex;a(this.rows).slice(1).each(function(j){if(parseInt(this.level,10)>=g&&parseInt(this.lft,10)>=f&&parseInt(this.lft,10)<=e){b.push(this)}});break;case"adjacency":break}});return b},getNodeAncestors:function(c){var b=[];this.each(function(){if(!this.grid||!this.p.treeGrid){return}var d=a(this).getNodeParent(c);while(d){b.push(d);d=a(this).getNodeParent(d)}});return b},isVisibleNode:function(c){var b=true;this.each(function(){var e=this;if(!e.grid||!e.p.treeGrid){return}var d=a(e).getNodeAncestors(c);a(d).each(function(){b=b&&this.expanded;if(!b){return false}})});return b},isNodeLoaded:function(c){var b;this.each(function(){var d=this;if(!d.grid||!d.p.treeGrid){return}if(c.loaded!==undefined){b=c.loaded}else{if(c.isLeaf||a(d).getNodeChildren(c).length>0){b=true}else{b=false}}});return b},expandNode:function(b){return this.each(function(){if(!this.grid||!this.p.treeGrid){return}if(!b.expanded){if(a(this).isNodeLoaded(b)){b.expanded=true;a("div.treeclick",b).removeClass("tree-plus").addClass("tree-minus")}else{b.expanded=true;a("div.treeclick",b).removeClass("tree-plus").addClass("tree-minus");this.p.treeANode=b.rowIndex;this.p.datatype=this.p.treedatatype;if(this.p.treeGridModel=="nested"){a(this).setGridParam({postData:{nodeid:b.id,n_left:b.lft,n_right:b.rgt,n_level:b.level}})}else{a(this).setGridParam({postData:{nodeid:b.id,parentid:b.parent_id,n_level:b.level}})}a(this).trigger("reloadGrid");if(this.p.treeGridModel=="nested"){a(this).setGridParam({postData:{nodeid:"",n_left:"",n_right:"",n_level:""}})}else{a(this).setGridParam({postData:{nodeid:"",parentid:"",n_level:""}})}}}})},collapseNode:function(b){return this.each(function(){if(!this.grid||!this.p.treeGrid){return}if(b.expanded){b.expanded=false;a("div.treeclick",b).removeClass("tree-minus").addClass("tree-plus")}})},SortTree:function(b){return this.each(function(){if(!this.grid||!this.p.treeGrid){return}var f,c,g,e=[],d=a(this).getRootNodes();d.sort(function(j,i){if(j.sortKey<i.sortKey){return -b}if(j.sortKey>i.sortKey){return b}return 0});for(f=0,c=d.length;f<c;f++){g=d[f];e.push(g);a(this).collectChildrenSortTree(e,g,b)}var h=this;a.each(e,function(i,j){a("tbody",h.grid.bDiv).append(j);j.sortKey=null})})},collectChildrenSortTree:function(b,d,c){return this.each(function(){if(!this.grid||!this.p.treeGrid){return}var g,e,h,f=a(this).getNodeChildren(d);f.sort(function(j,i){if(j.sortKey<i.sortKey){return -c}if(j.sortKey>i.sortKey){return c}return 0});for(g=0,e=f.length;g<e;g++){h=f[g];b.push(h);a(this).collectChildrenSortTree(b,h,c)}})},setTreeRow:function(c,d){var b,e=false;this.each(function(){var f=this;if(!f.grid||!f.p.treeGrid){return}e=a(f).setRowData(c,d)});return e},delTreeNode:function(b){return this.each(function(){var f=this;if(!f.grid||!f.p.treeGrid){return}var d=a(f).getInd(f.rows,b,true);if(d){var e=a(f).getNodeChildren(d);if(e.length>0){for(var c=0;c<e.length;c++){a(f).delRowData(e[c].id)}}a(f).delRowData(d.id)}})}})})(jQuery);