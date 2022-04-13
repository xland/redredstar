<script lang="ts">
  import type ClassicEditor from '@ckeditor/ckeditor5-build-classic'
  // import '@ckeditor/ckeditor5-build-classic/build/translations/zh-cn'
  // import '@ckeditor/ckeditor5-word-count/build/word-count'
  // import CKEditorInspector from '@ckeditor/ckeditor5-inspector'
  import { onMount } from 'svelte'
  import { db } from '../../common/db'
  import { eventer } from '../../common/eventer'
  import type { ArticleContentModel } from '../../model/ArticleContentModel'
  import { globalObjs } from '../Store/globalObjs'
  let content: ArticleContentModel
  let isFirstTimeChange = true
  let isHasChangeData = false
  let changeTimerSpan = 6000
  let autoSaveContent = async () => {
    if (isHasChangeData) {
      content.articleContent = globalObjs.editor.getData()
      await db('ArticleContent').update({ articleContent: content.articleContent }).where({ id: content.id })
      eventer.emit('articleContentSaved')
    }
    setTimeout(autoSaveContent, changeTimerSpan)
  }
  eventer.on('articleChange', async (articleId) => {
    if (isHasChangeData) {
      content.articleContent = globalObjs.editor.getData()
      await db('ArticleContent').update({ articleContent: content.articleContent }).where({ id: content.id })
    }
    content = await db('ArticleContent').where({ articleId }).first()
    if (globalObjs.editor) {
      globalObjs.editor.setData(content.articleContent)
    }
  })
  onMount(async () => {
    let editorElement = document.getElementById('ckEditor')
    let config = {
      removePlugins: ['MediaEmbedToolbar'],
    }
    globalObjs.editor = await ClassicEditor.create(editorElement, config)
    setTimeout(autoSaveContent, changeTimerSpan)
    if (content && content.articleContent) {
      globalObjs.editor.setData(content.articleContent)
    }
    globalObjs.editor.model.document.on('change:data', () => {
      if (isFirstTimeChange) {
        isFirstTimeChange = false
        return
      }

      eventer.emit('articleContentNeedSave')
      isHasChangeData = true
    })
    // console.log(Array.from(editor.ui.componentFactory.names()))
  })
</script>

<div id="ckEditor" />

<style lang="scss">
  #ckEditor {
  }
</style>
