export class Category {
  title: string
  id: string
  parentId: string
  order: number
  createTime: number
  updateTime: number
  subCategory: Category[]
  level: number
  isExpanded: boolean
  isSelected: boolean
}
