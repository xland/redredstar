export class CategoryModel {
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
  isNew: boolean = false
}
