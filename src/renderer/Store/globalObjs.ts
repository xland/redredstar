import type ClassicEditor from '@ckeditor/ckeditor5-build-classic'
class GlobalObjs {
  editor: ClassicEditor;
  [key: string]: any
}
export let globalObjs = new GlobalObjs()
