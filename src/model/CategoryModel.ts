import { BaseModel } from './BaseModel'

export class CategoryModel extends BaseModel {
  title: string = ''
  parentId: string
  orderNum = 1
  createTime: number
  updateTime: number
  levelNum: number
  isExpanded: boolean = false
  hasChild: boolean = false
  isSelected: boolean = false
  _isEdit: boolean = false
  _isNew: boolean = false
}
