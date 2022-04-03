<script lang="ts">
  import ClassicEditor from '@ckeditor/ckeditor5-build-classic'
  import '@ckeditor/ckeditor5-build-classic/build/translations/zh-cn'
  // import CKEditorInspector from '@ckeditor/ckeditor5-inspector'
  import { onMount } from 'svelte'
  import { globalObjs } from '../Store/globalObjs'
  onMount(async () => {
    let editorElement = document.getElementById('ckEditor')
    let config = {
      language: 'zh-cn',
      toolbar: {
        items: ['heading', 'bold', 'italic', 'blockQuote', 'link', 'imageUpload', 'indent', 'outdent', 'numberedList', 'bulletedList', 'insertTable'],
      },
    }
    globalObjs.editor = await ClassicEditor.create(editorElement, config)
    // console.log(Array.from(editor.ui.componentFactory.names()))
  })
</script>

<div id="ckEditor" />

<style lang="scss"></style>
