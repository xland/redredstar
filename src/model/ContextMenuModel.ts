export class ContextMenuModel {
  title: string
  subMenu?: ContextMenuModel[]
  onClick?: (e: MouseEvent) => void
}
