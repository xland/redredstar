import { BaseModel } from './BaseModel'

export class CategoryModel extends BaseModel {
  title: string = ''
  id: string
  parentId: string
  order: number
  createTime: number
  updateTime: number
  level: number
  isExpanded: boolean = false
  hasChild: boolean = false
  isSelected: boolean = false
  _isNew: boolean = false
}
