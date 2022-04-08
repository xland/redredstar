import { BaseModel } from './BaseModel'

export class ArticleModel extends BaseModel {
  categoryId: string
  title: string
  isSelected: boolean
  createTime: number
  updateTime: number
}
