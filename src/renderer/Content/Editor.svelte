<script lang="ts">
  import { onMount } from 'svelte'
  import { db } from '../../common/db'
  import { eventer } from '../../common/eventer'
  import type { ArticleContentModel } from '../../model/ArticleContentModel'
  import { globalObjs } from '../Store/globalObjs'
  import { schema } from 'prosemirror-schema-basic'
  import { EditorState } from 'prosemirror-state'
  import { EditorView } from 'prosemirror-view'
  import 'prosemirror-view/style/prosemirror.css'
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
    let editorElement = document.getElementById('articleEditorDiv')
    let state = EditorState.create({ schema })
    let view = new EditorView(editorElement, { state })
  })
</script>

<div id="articleEditorDiv" />

<style lang="scss">
</style>
