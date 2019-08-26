/**
 * @license Copyright (c) 2003-2019, CKSource - Frederico Knabben. All rights reserved.
 * For licensing, see LICENSE.md or https://ckeditor.com/legal/ckeditor-oss-license
 */

'use strict';

(function () {
	CKEDITOR.dialog.add('codeSnippet', function (editor) {
		var snippetLangs = editor._.codesnippet.langs,
			lang = editor.lang.codesnippet,
			clientHeight = document.documentElement.clientHeight,
			langSelectItems = [],
			snippetLangId;

		langSelectItems.push([editor.lang.common.notSet, '']);

		for (snippetLangId in snippetLangs)
			langSelectItems.push([snippetLangs[snippetLangId], snippetLangId]);

		// Size adjustments.
		var size = CKEDITOR.document.getWindow().getViewPaneSize(),
			// Make it maximum 800px wide, but still fully visible in the viewport.
			//width = Math.min( size.width - 70, 800 ),
			// Make it use 2/3 of the viewport height.
			//height = size.height / 1.5;
			//by liulun
			width = 472,
			height = 300;

		// Low resolution settings.// by liulun
		// if ( clientHeight < 650 ) {
		// 	height = clientHeight - 220;
		// }

		return {
			title: lang.title,
			minHeight: 200,
			width: 480,
			height: 360,
			resizable: CKEDITOR.DIALOG_RESIZE_NONE,
			onLoad: function () {//by liulun
				var labels = this.parts.contents.$.getElementsByTagName("label")
				labels[0].style = "display:none";
				labels[1].style = "display:none";
			},
			contents: [{
				id: 'info',
				elements: [{
						id: 'lang',
						type: 'select',
						label: lang.language,
						items: langSelectItems,
						setup: function (widget) {
							if (widget.ready && widget.data.lang)
								this.setValue(widget.data.lang);

							// The only way to have an empty select value in Firefox is
							// to set a negative selectedIndex.
							if (CKEDITOR.env.gecko && (!widget.data.lang || !widget.ready))
								this.getInputElement().$.selectedIndex = -1;
						},
						commit: function (widget) {
							widget.setData('lang', this.getValue());
						}
					},
					{
						id: 'code',
						type: 'textarea',
						label: lang.codeContents,
						setup: function (widget) {
							this.setValue(widget.data.code);
						},
						commit: function (widget) {
							widget.setData('code', this.getValue());
						},
						required: true,
						validate: CKEDITOR.dialog.validate.notEmpty(lang.emptySnippetError),
						inputStyle: 'cursor:auto;' +
							'width:' + width + 'px;' +
							'height:' + height + 'px !important;' + //by liulun important
							'tab-size:4;' +
							'text-align:left;',
						'class': 'cke_source'
					}
				]
			}]
		};
	});
}());