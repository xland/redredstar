import { writable } from 'svelte/store'
import { ArticleModel } from '../../model/ArticleModel'

let initArticle = () => {
  let articles = []
  for (let i = 0; i < 60; i++) {
    let article = new ArticleModel()
    article.id = i.toString()
    article.categoryId = i.toString()
    article.content = ''
    article.title = `文章标题文章标题文章标题${i}`
    article.isSelected = i === 58
    articles.push(article)
    articles = articles
  }
  return articles
}

export let articleStore = writable<ArticleModel[]>([], (set) => {
  let articles = initArticle()
  set(articles)
  return () => {
    console.log('articleStore clean')
  }
})
