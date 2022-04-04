<script lang="ts">
  import { onMount } from 'svelte'

  import { Article } from '../../../model/Article'

  let articles: Article[] = []
  onMount(() => {
    for (let i = 0; i < 60; i++) {
      let article = new Article()
      article.id = i.toString()
      article.categoryId = i.toString()
      article.content = ''
      article.title = `文章标题文章标题文章标题${i}`
      articles.push(article)
      articles = articles
    }
  })
</script>

<div class="titleList">
  {#each articles as article (article.id)}
    <div class="articleTitle">{article.title}</div>
  {/each}
</div>

<style lang="scss">
  .titleList {
    background: rgb(241, 248, 255);
    border-right: 1px solid rgb(230, 231, 232);
    flex: 1;
    overflow-y: scroll;
  }
  .articleTitle {
    height: 32px;
    overflow: hidden;
    text-overflow: ellipsis;
    line-height: 32px;
    padding-left: 12px;
  }
  .articleTitle:hover {
    background: rgb(33, 136, 255);
    color: #fff;
  }
</style>
