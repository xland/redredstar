/**
 * @license Copyright (c) 2003-2019, CKSource - Frederico Knabben. All rights reserved.
 * For licensing, see https://ckeditor.com/legal/ckeditor-oss-license
 */

CKEDITOR.editorConfig = function (config) {
	config.language = 'zh-cn';
	config.toolbar = [{
			name: 'basicstyles',
			items: ['Bold', 'Italic', 'Underline', 'Strike', 'TextColor', 'BGColor', 'Subscript', 'Superscript']
		},
		{
			name: 'paragraph',
			items: ['NumberedList', 'BulletedList', 'Outdent', 'Indent', 'Blockquote', 'RemoveFormat','-', 'JustifyLeft', 'JustifyCenter', 'JustifyRight', 'JustifyBlock', '-', 'BidiLtr', 'BidiRtl']
		},
		{
			name: 'links',
			items: ['Link', 'Unlink', 'Anchor']
		},
		{
			name: 'insert',
			items: ['Image', 'Table', 'HorizontalRule', 'SpecialChar', 'CodeSnippet', 'Mathjax']
		},
		'/',
		{
			name: 'styles',
			items: ['Styles', 'Format', 'Font', 'FontSize']
		},
		{
			name: 'document',
			items: ['Print','Find', '-','Templates', '-', 'CreateDiv', 'ShowBlocks', 'Source', ]
		},
	];
	config.enterMode = CKEDITOR.ENTER_BR;
	config.shiftEnterMode = CKEDITOR.ENTER_BR;
	config.removePlugins = 'elementspath,maximize,newpage,language,about,iframe,save,flash,image,scayt';
	config.extraPlugins = 'codesnippet,mathjax,image2,table,tabletools,tableresize,tableselection,find';
	config.mathJaxLib = 'https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.4/MathJax.js?config=TeX-AMS_HTML';
	//config.allowedContent = true; 
	config.extraAllowedContent = 'dl dt dd img[id,data-*]';
	config.imageUploadUrl = '/useless/url'
	config.resize_enabled = false;
	config.height = "auto";
};
//config.removeButtons = "About,Iframe,PageBreak,Form,TextField,Textarea,Button,"
//config.removePlugins = 'elementspath,save,image,flash,iframe,link,smiley,tabletools,find,pagebreak,templates,about,maximize,showblocks,newpage,language';
//config.removeButtons = 'Copy,Cut,Paste,Undo,Redo,Print,Form,TextField,Textarea,Button,SelectAll,NumberedList,BulletedList,CreateDiv,Table,PasteText,PasteFromWord,Select,HiddenField';
// toolbar: [
//     { name: 'document', groups: [ 'mode', 'document', 'doctools' ], items: [ 'Source', '-', 'Save', 'NewPage', 'Preview', 'Print', '-', 'Templates' ] },
//     { name: 'clipboard', groups: [ 'clipboard', 'undo' ], items: [ 'Cut', 'Copy', 'Paste', 'PasteText', 'PasteFromWord', '-', 'Undo', 'Redo' ] },
//     { name: 'editing', groups: [ 'find', 'selection', 'spellchecker' ], items: [ 'Find', 'Replace', '-', 'SelectAll', '-', 'Scayt' ] },
//     { name: 'forms', items: [ 'Form', 'Checkbox', 'Radio', 'TextField', 'Textarea', 'Select', 'Button', 'ImageButton', 'HiddenField' ] },
//     '/',
//     { name: 'basicstyles', groups: [ 'basicstyles', 'cleanup' ], items: [ 'Bold', 'Italic', 'Underline', 'Strike', 'Subscript', 'Superscript', '-', 'RemoveFormat' ] },
//     { name: 'paragraph', groups: [ 'list', 'indent', 'blocks', 'align', 'bidi' ], items: [ 'NumberedList', 'BulletedList', '-', 'Outdent', 'Indent', '-', 'Blockquote', 'CreateDiv', '-', 'JustifyLeft', 'JustifyCenter', 'JustifyRight', 'JustifyBlock', '-', 'BidiLtr', 'BidiRtl', 'Language' ] },
//     { name: 'links', items: [ 'Link', 'Unlink', 'Anchor' ] },
//     { name: 'insert', items: [ 'Image', 'Flash', 'Table', 'HorizontalRule', 'Smiley', 'SpecialChar', 'PageBreak', 'Iframe' ] },
//     '/',
//     { name: 'styles', items: [ 'Styles', 'Format', 'Font', 'FontSize' ] },
//     { name: 'colors', items: [ 'TextColor', 'BGColor' ] },
//     { name: 'tools', items: [ 'Maximize', 'ShowBlocks' ] },
//     { name: 'others', items: [ '-' ] },
//     { name: 'about', items: [ 'About' ] }
// ]