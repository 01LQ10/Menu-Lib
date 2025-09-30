def parse_menu_structure(file_path):
    """
    解析菜单结构文件
    格式：
    [DIR]root
    -[DIR]sets
    --[VAL]bright(bright)
    --[FUN]openled(openled)
    """
    with open(file_path, 'r', encoding='utf-8') as f:
        lines = f.readlines()
    
    root = None
    stack = []  # 保存各级节点和层级
    
    for line_num, line in enumerate(lines, 1):
        line = line.strip()
        if not line or line.startswith('#'):  # 跳过空行和注释
            continue
            
        # 解析层级（开头的 '-' 数量）
        level = 0
        for char in line:
            if char == '-':
                level += 1
            else:
                break
                
        content = line[level:]  # 去掉层级标记后的内容
        
        # 解析节点信息 [TYPE]name(pointer)
        import re
        match = re.match(r'\[(\w+)\](.+?)(?:\((.+?)\))?$', content)
        if not match:
            print(f"警告: 第{line_num}行格式错误: {line}")
            continue
            
        node_type, name, pointer = match.groups()
        
        # 处理默认指针：非DIR类型且没有指定指针时，使用节点名称
        if pointer is None and node_type != 'DIR':
            # 使用节点名称作为默认指针，去掉空格和特殊字符
            pointer = name.strip().replace(' ', '_')
        
        node = {
            'name': name.strip(),
            'type': f"NODETYPE_{node_type}",
            'pointer': pointer,
            'level': level,
            'children': []
        }
        
        if level == 0:
            # 根节点
            root = node
            stack = [(level, node)]
        else:
            # 找到父节点
            while stack and stack[-1][0] >= level:
                stack.pop()
                
            if stack:
                parent_level, parent_node = stack[-1]
                parent_node['children'].append(node)
                stack.append((level, node))
            else:
                print(f"错误: 第{line_num}行没有找到父节点: {line}")
    
    return root

def calculate_statistics(node):
    """计算节点统计信息"""
    stats = {
        'total_nodes': 0,
        'max_depth': 0
    }
    
    def traverse(node, depth):
        stats['total_nodes'] += 1
        stats['max_depth'] = max(stats['max_depth'], depth)
        
        for child in node.get('children', []):
            traverse(child, depth + 1)
    
    if node:
        traverse(node, 0)
    
    return stats

def generate_c_code(menu_data, output_file):
    """生成C代码 - 使用嵌套函数调用格式"""
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write('// Auto-generated menu structure\n\n')
        f.write('#include "MenuCore.h"\n\n')
        
        # 计算统计信息
        stats = calculate_statistics(menu_data)
        
        # 输出统计宏定义
        f.write('// Menu Statistics\n')
        f.write(f'#define MENU_TOTAL_NODES\t{stats["total_nodes"]}\n')
        f.write(f'#define MENU_MAX_DEPTH\t\t{stats["max_depth"]}\n\n')
        
        def build_node_expression(node, indent_level=0):
            """递归构建节点表达式"""
            indent = '\t' * indent_level
            
            if node['children']:
                # 有子节点的情况
                children_exprs = []
                for child in node['children']:
                    children_exprs.append(build_node_expression(child, indent_level + 1))
                
                # 构建 SetBranch 表达式
                branch_args = ', \n'.join(children_exprs)
                
                # 构建完整节点表达式
                if node['pointer']:
                    return f"{indent}SetNode({node['type']}, \"{node['name']}\", SetBranch(\n{branch_args}\n{indent}))"
                else:
                    return f"{indent}SetNode({node['type']}, \"{node['name']}\", SetBranch(\n{branch_args}\n{indent}))"
            else:
                # 叶子节点
                if node['pointer']:
                    return f"{indent}SetNode({node['type']}, \"{node['name']}\", {node['pointer']})"
                else:
                    return f"{indent}SetNode({node['type']}, \"{node['name']}\", NULL)"
        
        # 生成根节点赋值语句
        root_expr = build_node_expression(menu_data)
        f.write(f"Node_t *root = {root_expr};\n")

# 使用示例
if __name__ == "__main__":
    # 解析菜单文件
    menu_data = parse_menu_structure('menu_structure.txt')
    
    # 打印统计信息（控制台输出）
    stats = calculate_statistics(menu_data)
    print(f"菜单统计:")
    print(f"  总节点数: {stats['total_nodes']}")
    print(f"  最大深度: {stats['max_depth']}")
    
    # 生成C代码
    generate_c_code(menu_data, 'menu_structure.c')
    print("C代码已生成到 menu_structure.c")