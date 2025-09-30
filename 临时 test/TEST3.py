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
        'max_depth': 0,
        'node_list': []  # 按遍历顺序存储所有节点
    }
    
    def traverse(node, depth):
        stats['total_nodes'] += 1
        stats['max_depth'] = max(stats['max_depth'], depth)
        stats['node_list'].append(node)
        
        for child in node.get('children', []):
            traverse(child, depth + 1)
    
    if node:
        traverse(node, 0)
    
    return stats

def generate_flash_based_c_code(menu_data, output_file):
    """生成基于Flash存储的C代码 - 常量存储在Flash中"""
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write('// Auto-generated menu structure (Flash Storage)\n\n')
        f.write('#include "menu.h"\n\n')
        
        # 计算统计信息
        stats = calculate_statistics(menu_data)
        
        # 输出统计宏定义
        f.write('// Menu Statistics\n')
        f.write(f'#define MENU_TOTAL_NODES\t{stats["total_nodes"]}\n')
        f.write(f'#define MENU_MAX_DEPTH\t\t{stats["max_depth"]}\n\n')
        
        # 定义常量节点数组（存储在Flash中）
        f.write('// Menu Node Array (Stored in Flash)\n')
        f.write('const Node_t menu_nodes[MENU_TOTAL_NODES] = {\n')
        
        for i, node in enumerate(stats['node_list']):
            # 查找第一个子节点的索引
            first_child_index = -1
            if node['children']:
                first_child_index = stats['node_list'].index(node['children'][0])
            
            # 查找下一个兄弟节点的索引
            next_sibling_index = -1
            parent = None
            for potential_parent in stats['node_list']:
                if node in potential_parent.get('children', []):
                    parent = potential_parent
                    break
            
            if parent:
                children = parent['children']
                node_index = children.index(node)
                if node_index < len(children) - 1:
                    next_sibling_index = stats['node_list'].index(children[node_index + 1])
            
            # 生成节点初始化代码
            f.write(f'\t{{\n')
            f.write(f'\t\t.name = "{node["name"]}",\n')
            f.write(f'\t\t.type = {node["type"]},\n')
            
            # 设置pointer：如果是目录指向第一个子节点，否则指向函数/变量
            if node['children']:
                f.write(f'\t\t.pointer = (void*)&menu_nodes[{first_child_index}],\n')
            elif node['pointer']:
                f.write(f'\t\t.pointer = (void*){node["pointer"]},\n')
            else:
                f.write(f'\t\t.pointer = NULL,\n')
            
            # 设置nextNode
            if next_sibling_index != -1:
                f.write(f'\t\t.nextNode = (Node_t*)&menu_nodes[{next_sibling_index}],\n')
            else:
                f.write(f'\t\t.nextNode = NULL,\n')
            
            f.write(f'\t}}')
            
            if i < len(stats['node_list']) - 1:
                f.write(',')
            f.write(f'\t// {node["name"]}\n')
        
        f.write('};\n\n')
        
        # 节点索引映射
        f.write('// Node Index Mapping\n')
        f.write('enum {\n')
        for i, node in enumerate(stats['node_list']):
            var_name = node['name'].replace(' ', '_').upper()
            f.write(f'\tMENU_INDEX_{var_name} = {i},\n')
        f.write('};\n\n')
        
        # 根节点指针（也存储在Flash中）
        f.write('// Root Node Pointer (Stored in Flash)\n')
        f.write('const Node_t *const root = &menu_nodes[0];\n\n')
        
        # 节点引用宏（使用常量指针）
        f.write('// Node Reference Macros (Const Pointers)\n')
        for i, node in enumerate(stats['node_list']):
            var_name = node['name'].replace(' ', '_').lower()
            f.write(f'#define {var_name}_node\t(&menu_nodes[{i}])\n')
        f.write('\n')

def generate_flash_based_c_code_compact(menu_data, output_file):
    """紧凑版本的Flash存储代码生成"""
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write('// Auto-generated menu structure (Flash Storage - Compact)\n\n')
        f.write('#include "menu.h"\n\n')
        
        stats = calculate_statistics(menu_data)
        
        f.write('// Menu Statistics\n')
        f.write(f'#define MENU_TOTAL_NODES\t{stats["total_nodes"]}\n')
        f.write(f'#define MENU_MAX_DEPTH\t\t{stats["max_depth"]}\n\n')
        
        # 紧凑的数组定义
        f.write('// Compact Menu Node Array in Flash\n')
        f.write('const Node_t menu_nodes[MENU_TOTAL_NODES] = {\n')
        
        for i, node in enumerate(stats['node_list']):
            # 计算指针和nextNode
            first_child_ptr = 'NULL'
            if node['children']:
                first_child_index = stats['node_list'].index(node['children'][0])
                first_child_ptr = f'&menu_nodes[{first_child_index}]'
            elif node['pointer']:
                first_child_ptr = f'(void*){node["pointer"]}'
            
            next_node_ptr = 'NULL'
            parent = None
            for potential_parent in stats['node_list']:
                if node in potential_parent.get('children', []):
                    parent = potential_parent
                    break
            if parent:
                children = parent['children']
                node_index = children.index(node)
                if node_index < len(children) - 1:
                    next_sibling_index = stats['node_list'].index(children[node_index + 1])
                    next_node_ptr = f'&menu_nodes[{next_sibling_index}]'
            
            f.write(f'\t{{ "{node["name"]}", {node["type"]}, {first_child_ptr}, {next_node_ptr} }},\n')
        
        f.write('};\n\n')
        
        f.write('const Node_t *const root = &menu_nodes[0];\n\n')

# 使用示例
if __name__ == "__main__":
    # 解析菜单文件
    menu_data = parse_menu_structure('menu_structure.txt')
    
    # 打印统计信息
    stats = calculate_statistics(menu_data)
    print(f"菜单统计:")
    print(f"  总节点数: {stats['total_nodes']}")
    print(f"  最大深度: {stats['max_depth']}")
    
    # 生成Flash存储的C代码
    generate_flash_based_c_code(menu_data, 'menu_structure_flash.c')
    print("Flash存储的C代码已生成到 menu_structure_flash.c")
    
    # 可选：生成紧凑版本
    generate_flash_based_c_code_compact(menu_data, 'menu_structure_flash_compact.c')
    print("紧凑版本已生成到 menu_structure_flash_compact.c")