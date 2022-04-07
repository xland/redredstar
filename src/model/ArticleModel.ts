import { BaseModel } from './BaseModel'

export class ArticleModel extends BaseModel {
  categoryId: string
  title: string
  content: string
  isSelected: boolean
}
